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

    bool EnumTextConversionStrategy::fromStringImpl(const QString &s, Type &element) const
    {

    }


} // namespace Entity::Converters
