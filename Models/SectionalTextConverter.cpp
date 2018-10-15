/*****************************************************************************
**
** Copyright (C) 2018 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 20 .
**
** This file is part of Q-UML (UML tool for Qt).
**
** Q-UML is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Q-UML is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.

** You should have received a copy of the GNU Lesser General Public License
** along with Q-UML.  If not, see <http://www.gnu.org/licenses/>.
**
*****************************************************************************/

#include "SectionalTextConverter.hpp"

#include <functional>
#include <optional>

#include <range/v3/algorithm/transform.hpp>
#include <range/v3/view/slice.hpp>

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>

#include <DB/ITypeSearcher.h>
#include <Common/BasicElement.h>

#include <Entity/Enum.h>

#include <Utility/helpfunctions.h>

#include "IMessenger.h"
#include "QtHelpers.h"

namespace Models
{
    using namespace boost;

    class ConvException : public std::exception
    {
        Q_DECLARE_TR_FUNCTIONS(ConvException)

    public:
        ConvException(QString const& shortMsg, QString const& descr = QString::null)
            : m_ShortMsg(shortMsg)
            , m_Descr(descr)
        {}

        void writeMsg(IMessenger &messenger) const
        {
            messenger.addMessage(MessageType::Error, m_ShortMsg, m_Descr);
        }

    private:
        QString m_ShortMsg;
        QString m_Descr;
    };

    /**
     * @brief SectionalTextConverter::SectionalTextConverter
     * @param parent
     */
    SectionalTextConverter::SectionalTextConverter(SharedMessenger const &messenger)
        : m_Messenger(G_ASSERT(messenger))
    {
    }

    static DB::SharedTypeSearchers lockTS(DB::WeakTypeSearchersSet const &ts)
    {
        DB::SharedTypeSearchers result;
        result.reserve(ts.size());

        ranges::transform(ts, ranges::back_inserter(result), [](auto &&t) { return t.lock(); });

        return result;
    }

    static Entity::SharedType typeByID(Common::ID const &id, DB::SharedTypeSearchers const &ts)
    {
        for (auto &&s : ts)
            if (auto t = s->typeByID(id))
                return t;

        return nullptr;
    }

    static QString enumToString(Entity::Type const &elem, DB::WeakTypeSearchersSet const &ts,
                                Models::IMessenger &messenger)
    {
        auto lockedTS = lockTS(ts);
        if (lockedTS.isEmpty())
            return QString::null;

        auto const & e = elem.to<Entity::Enum>();

        QString result("enum");

        if (e.isStrong())
            result.append(" class");

        if (e.name().isEmpty()) {
            messenger.addMessage(Models::MessageType::Error, "Enum name is empty");
            return QString::null;
        }
        result.append(" " + e.name());

        if (auto id = e.enumTypeId(); id.isValid()) {
            if (auto type = typeByID(id, lockedTS)) {
                result.append(" ").append(type->name());
            } else {
                messenger.addMessage(Models::MessageType::Error, "Wrong type ID",
                                     "Cannot find the following type id: " + id.toString());
                return QString::null;
            }
        }

        result.append("\n");

        for (auto && enumerator : e.enumerators()) {
            QString name = enumerator->name();
            if (name.isEmpty()) {
                messenger.addMessage(Models::MessageType::Error, "Enumerator name is empty");
                return QString::null;
            }

            result.append(enumerator->name());
            if (auto val = enumerator->value())
                result.append(QString(" %1").arg(Entity::Enumerator::valToString(val.value())));
            result.append("\n");
        }

        return result;
    }

    using ToStringConverter =
        std::function<QString(Entity::Type const&,
                      DB::WeakTypeSearchersSet const&,
                      Models::IMessenger &messenger)>;
    static const QHash<size_t, ToStringConverter> toStrConvById =
    {
        {Entity::Enum::staticHashType(), &enumToString},
    };

    /**
     * @brief SectionalTextConverter::toString
     * @param element
     * @return
     */
    QString SectionalTextConverter::toString(Entity::Type const& element) const noexcept
    {
        try {
            if (auto it = toStrConvById.find(element.hashType()); it != toStrConvById.end())
                return (*it)(element, m_TypeSearchers, *m_Messenger);
            else
                m_Messenger->addMessage(MessageType::Error, tr("Cannot get string representation"),
                                        tr("There is no an appropriate converter"));
        } catch (ConvException const& e) {
            e.writeMsg(*m_Messenger);
        } catch (...) {
            m_Messenger->addMessage(MessageType::Error, tr("Unexpected error"));
        }

        return QString::null;
    }

    static Common::ID typeIDByName(QStringRef const& name, DB::WeakTypeSearchersSet const& searchers)
    {
        if (name.isEmpty())
            return Common::ID::nullID();

        for (auto && weakSearcher: searchers)
            if (auto searcher = weakSearcher.lock())
                if (auto type = searcher->typeByName(name.toString()))
                    return type->id();

        throw ConvException(ConvException::tr("Wrong type"));
    }

    static auto baseFromStr(QStringRef baseRef)
    {
        if (baseRef.isEmpty())
            return Entity::Enumerator::Dec;

        if (baseRef == QStringLiteral("0"))
            return Entity::Enumerator::Oct;

        return Entity::Enumerator::Hex;
    }

    static auto enumValueFromStr(QStringRef valueRef, QStringRef baseRef)
    {
        Entity::Enumerator::OptionalValue result;

        if (!valueRef.isEmpty()) {
            auto base = baseFromStr(baseRef);

            bool convRes = false;
            auto value = valueRef.toInt(&convRes, base);
            if (!convRes)
                throw ConvException(ConvException::tr("Cannot convert enumerator value: %1")
                                    .arg(valueRef.toString()));

            result = std::make_pair(value, base);
        }

        return result;
    }

    template<class Collection>
    void readEnumerators(Collection const& lines, Entity::Enum &dstEnum)
    {
        static const QString nameGroup   = "name" ;
        static const QString valueGroup  = "value";
        static const QString nsGroup     = "ns"   ;

        static const QString enumeratorPattern =
            "^(?<" + nameGroup + ">\\w+)"
            "(?:\\s+(?<" + nsGroup + ">(?:0|0x))?(?<" + valueGroup + ">[0-9A-Fa-f]+))?[\\r\\n]*?$";

        for (auto &&enumeratorLine: lines) {
            QRegularExpression enumeratorRe(enumeratorPattern);
            auto enumeratorReMach = enumeratorRe.match(enumeratorLine);
            if (enumeratorReMach.hasMatch()) {
                auto enumerator = dstEnum.addElement(enumeratorReMach.captured(nameGroup));

                if (auto valueRef = enumeratorReMach.capturedRef(valueGroup); !valueRef.isEmpty())
                    enumerator->setValue(
                        enumValueFromStr(valueRef, enumeratorReMach.capturedRef(nsGroup)));
            } else
                throw ConvException(ConvException::tr("Cannot read enumerator"));
        }
    }

    void readEnumHeader(QStringRef header, DB::WeakTypeSearchersSet const& ts, Entity::Enum &dstEnum)
    {
        static const QString scopedGroup = "isScoped";
        static const QString nameGroup   = "name"    ;
        static const QString typeGroup   = "type"    ;

        static const QString headerPattern =
            "^enum(?:\\s+(?<" + scopedGroup + ">class))?"
            "\\s+(?<" + nameGroup + ">\\w+)"
            "(?:\\s+(?<" + typeGroup + ">\\w+))?"
            "[\\r\\n]*?$";

        QRegularExpression headerRe(headerPattern);
        if (auto headerReMach = headerRe.match(header); headerReMach.hasMatch()) {
            dstEnum.setStrongStatus(!headerReMach.capturedRef(scopedGroup).isEmpty());
            dstEnum.setName(headerReMach.captured(nameGroup));

            auto typeNameRef = headerReMach.capturedRef(typeGroup);
            dstEnum.setEnumTypeId(typeIDByName(typeNameRef, ts));
        } else
            throw ConvException(ConvException::tr("Cannot read enum header"));
    }

    static void enumFromString(QString const& in, Entity::Type &e,
                               DB::WeakTypeSearchersSet const& ts)
    {
        auto lines = in.splitRef("\n", QString::SkipEmptyParts);
        if (lines.isEmpty())
            throw ConvException(ConvException::tr("Cannot get enum from this string"),
                                ConvException::tr("The string is empty"));

        Entity::Enum tmpEnum(e.to<Entity::Enum>());

        readEnumHeader(lines[0], ts, tmpEnum);

        if (lines.length() > 1)
            readEnumerators(lines | ranges::view::slice(1, lines.length()), tmpEnum);

        e.to<Entity::Enum>() = std::move(tmpEnum);
    }

    using FromStringConverter = std::function<void(QString const&, Entity::Type &,
                                              DB::WeakTypeSearchersSet const&)>;
    static const QHash<size_t, FromStringConverter> fromStrConvById =
    {
        {Entity::Enum::staticHashType(), &enumFromString},
    };

    /**
     * @brief SectionalTextConverter::fromString
     * @param s
     * @param element
     * @return
     */
    bool SectionalTextConverter::fromString(QString const& s,
                                            Entity::Type &element) const noexcept
    {
        bool success = false;

        try {
            if (auto it = fromStrConvById.find(element.hashType()); it != fromStrConvById.end()) {
                (*it)(s, element, m_TypeSearchers);
                success = true;
            } else
                m_Messenger->addMessage(MessageType::Error, tr("Cannot convert the type from string"),
                                        tr("There is no an appropriate converter"));
        } catch (ConvException const& e) {
            e.writeMsg(*m_Messenger);
        } catch (...) {
            m_Messenger->addMessage(MessageType::Error, tr("Unexpected error"));
        }

        return success;
    }

    /**
     * @brief SectionalTextConverter::registerTypeSearcher
     * @param typeSearcher
     */
    void SectionalTextConverter::registerTypeSearcher(DB::SharedTypeSearcher const& typeSearcher)
    {
        m_TypeSearchers << typeSearcher;
    }

    /**
     * @brief SectionalTextConverter::unregisterTypeSearcher
     * @param typeSearcher
     */
    void SectionalTextConverter::unregisterTypeSearcher(DB::SharedTypeSearcher const& typeSearcher)
    {
        m_TypeSearchers.remove(typeSearcher);
    }

} // namespace Models
