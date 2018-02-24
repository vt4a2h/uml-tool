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

#include <boost/range/algorithm/transform.hpp>
#include <boost/range/algorithm/find.hpp>
#include <boost/range/algorithm_ext/erase.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <DB/ITypeSearcher.h>
#include <Common/BasicElement.h>

#include <Entity/Enum.h>

#include "IMessenger.h"
#include "QtHelpers.h"

namespace Models
{

    using namespace boost;

    /**
     * @brief SectionalTextConverter::SectionalTextConverter
     * @param parent
     */
    SectionalTextConverter::SectionalTextConverter(SharedMessenger const & messenger)
        : m_Messenger(G_ASSERT(messenger))
    {
    }

    template <class T>
    T const & to(Common::BasicElement const & elem)
    {
        Q_ASSERT(T::staticHashType() == elem.hashType());
        return static_cast<T const &>(elem);
    }

    static DB::SharedTypeSearchers lockTS(DB::WeakTypeSearchers const & ts)
    {
        DB::SharedTypeSearchers result;
        result.reserve(ts.size());

        range::transform(ts, std::back_inserter(result), [](auto && t) { return t.lock(); });

        return result;
    }

    static Entity::SharedType typeByID(Common::ID const & id, DB::SharedTypeSearchers const & ts)
    {
        for (auto && s : ts)
            if (auto t = s->typeByID(id))
                return t;

        return nullptr;
    }

    static QString enumToString(Common::BasicElement const & elem, DB::WeakTypeSearchers const & ts)
    {
        auto lockedTS = lockTS(ts);
        if (lockedTS.isEmpty())
            return QString::null;

        auto e = to<Entity::Enum>(elem);

        QString result;

        if (e.isStrong())
            result.append("class ");

        result.append(e.name());

        if (auto id = e.enumTypeId(); id.isValid())
            if (auto type = typeByID(id, lockedTS))
                result.append(" ").append(type->name());

        result.append("\n");

        for (auto && enumerator : e.enumerators())
            result.append(QString("%1 %2\n").arg(enumerator->name()).arg(enumerator->value()));

        return result;
    }

    using ToStringConverter =
        std::function<QString(Common::BasicElement const&, DB::WeakTypeSearchers const &)>;
    static const QHash<size_t, ToStringConverter> toStrConvById =
    {
        {Entity::Enum::staticHashType(), &enumToString},
    };

    /**
     * @brief SectionalTextConverter::toString
     * @param element
     * @return
     */
    QString SectionalTextConverter::toString(Common::BasicElement const & element) const noexcept
    {
        if (auto it = toStrConvById.find(element.hashType()); it != toStrConvById.end())
            return (*it)(element, m_TypeSearchers);
        else
            m_Messenger->addMessage(MessageType::Error, tr("Cannot get string representation"),
                                    tr("There is no an appropriate converter"));

        return QString::null;
    }

    static void enumFromString(QString const & in, Common::BasicElement & e,
                               DB::WeakTypeSearchers const &ts, IMessenger const & messenger)
    {

    }

    using FromStringConverter = std::function<void(QString const &, Common::BasicElement &,
                                              DB::WeakTypeSearchers const &, IMessenger const &)>;
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
    void SectionalTextConverter::fromString(QString const & s,
                                            Common::BasicElement & element) const noexcept
    {
        if (auto it = fromStrConvById.find(element.hashType()); it != fromStrConvById.end())
            (*it)(s, element, m_TypeSearchers, *m_Messenger);
        else
            m_Messenger->addMessage(MessageType::Error, tr("Cannot get string representation"),
                                    tr("There is no an appropriate converter"));
    }

    /**
     * @brief SectionalTextConverter::registerTypeSearcher
     * @param typeSearcher
     */
    void SectionalTextConverter::registerTypeSearcher(DB::SharedTypeSearcher const & typeSearcher)
    {
        if (auto it = range::find(m_TypeSearchers, typeSearcher); it == m_TypeSearchers.end())
            m_TypeSearchers << typeSearcher;
    }

    /**
     * @brief SectionalTextConverter::unregisterTypeSearcher
     * @param typeSearcher
     */
    void SectionalTextConverter::unregisterTypeSearcher(DB::SharedTypeSearcher const & typeSearcher)
    {
        range::remove_erase(m_TypeSearchers, typeSearcher);
    }

} // namespace Models