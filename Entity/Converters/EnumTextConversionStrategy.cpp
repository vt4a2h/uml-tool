/*****************************************************************************
**
** Copyright (C) 2018 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 17/10/2018.
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
#include "EnumTextConversionStrategy.hpp"

#include <QRegularExpression>

#include <range/v3/view/slice.hpp>

#include <Entity/Enum.h>

#include <Models/IMessenger.h>

namespace Entity::Converters {

    static void addEnumerators(const Entity::Enum &e, QString &result)
    {
        for (auto &&enumerator : e.enumerators()) {
            QString name = enumerator->name();
            if (name.isEmpty())
                throw Models::MessageException(
                          Models::MessageType::Error,
                          EnumTextConversionStrategy::tr("Enumerator name is empty"));

            result.append(enumerator->name());
            if (auto val = enumerator->value())
                result.append(QString(" %1").arg(Entity::Enumerator::valToString(val.value())));
            result.append("\n");
        }
    }

    static void addStrongStatus(const Entity::Enum &e, QString &result)
    {
        if (e.isStrong())
            result.append(" class");
    }

    static void addName(const Entity::Enum &e, QString &result)
    {
        if (e.name().isEmpty())
            throw Models::MessageException(
                      Models::MessageType::Error,
                      EnumTextConversionStrategy::tr("Enum name is empty"));

        result.append(" " + e.name());

    }

    void EnumTextConversionStrategy::addEnumTypename(const Enum &e, QString &result) const
    {
        if (auto id = e.enumTypeId(); id.isValid()) {
            if (auto type = typeByID(id))
                result.append(" ").append(type->name());
            else
                throw Models::MessageException(
                          Models::MessageType::Error,
                          tr("Wrong type ID"),
                          tr("Cannot find the following type id: ") + id.toString());
        }
        result.append("\n");
    }

    QString EnumTextConversionStrategy::toStringImpl(const Type &element) const
    {
        const auto &e = element.to<Entity::Enum>();

        QString result("enum");

        addStrongStatus(e, result);
        addName(e, result);
        addEnumTypename(e, result);
        addEnumerators(e, result);

        return result;
    }

    void EnumTextConversionStrategy::readEnumHeader(QStringRef header, Entity::Enum &dstEnum) const
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

            auto typeName = headerReMach.captured(typeGroup);
            dstEnum.setEnumTypeId(typeIdByName(typeName));
        } else
            throw Models::MessageException(Models::MessageType::Error,
                                           tr("Cannot read enum header: ") + header);
    }

    static auto split(const QString &s)
    {
        auto lines = s.splitRef("\n", QString::SkipEmptyParts);
        if (lines.isEmpty())
            throw Models::MessageException(Models::MessageType::Error,
                                           EnumTextConversionStrategy::tr("Cannot get enum from this string"),
                                           EnumTextConversionStrategy::tr("The string is empty"));

        return lines;
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
                throw Models::MessageException(
                          Models::MessageType::Error,
                          EnumTextConversionStrategy::tr("Cannot convert enumerator value: ") + valueRef);

            result = std::make_pair(value, base);
        }

        return result;
    }

    template<class Collection>
    void readEnumerators(const Collection &lines, Entity::Enum &dstEnum)
    {
        for (auto &&e: dstEnum.enumerators())
            if (e)
                dstEnum.removeEnumerator(e->name());

        if (lines.length() <= 1)
            return;

        static const QString nameGroup   = "name" ;
        static const QString valueGroup  = "value";
        static const QString nsGroup     = "ns"   ;

        static const QString enumeratorPattern =
            "^(?<" + nameGroup + ">\\w+)"
            "(?:\\s+(?<" + nsGroup + ">(?:0|0x))?(?<" + valueGroup + ">[0-9A-Fa-f]+))?[\\r\\n]*?$";

        for (auto &&enumeratorLine: lines | ranges::view::slice(1, lines.length())) {
            QRegularExpression enumeratorRe(enumeratorPattern);
            auto enumeratorReMach = enumeratorRe.match(enumeratorLine);
            if (enumeratorReMach.hasMatch()) {
                auto enumerator = dstEnum.addElement(enumeratorReMach.captured(nameGroup));

                if (auto valueRef = enumeratorReMach.capturedRef(valueGroup); !valueRef.isEmpty())
                    enumerator->setValue(
                        enumValueFromStr(valueRef, enumeratorReMach.capturedRef(nsGroup)));
            } else
                throw Models::MessageException(
                          Models::MessageType::Error,
                          EnumTextConversionStrategy::tr("Cannot read enumerator: ") + enumeratorLine);
        }
    }

    bool EnumTextConversionStrategy::fromStringImpl(const QString &s, Type &element) const
    {
        Entity::Enum tmpEnum(element.to<Entity::Enum>());

        auto lines = split(s);
        readEnumHeader(lines[0], tmpEnum);
        readEnumerators(lines, tmpEnum);

        swap(element.to<Entity::Enum>(), tmpEnum);

        return true;
    }

} // namespace Entity::Converters
