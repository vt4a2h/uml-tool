/*****************************************************************************
**
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 29/10/2014.
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

#pragma once

#include <QString>

#include <common/id.h>

#include <entity/entity_types.hpp>

class QJsonObject;

namespace relationship {

    enum Multiplicity : int;

    /**
     * @brief The Node class
     */
    class Node
    {
    public:
        Node();
        Node(const entity::SharedType &type,
             Multiplicity multiplicity = static_cast<Multiplicity>(0));

        friend bool operator ==(const Node &lhs, const Node &rhs);

        Multiplicity multiplicity() const;
        void setMultiplicity(const Multiplicity &multiplicity);

        QString description() const;
        void setDescription(const QString &description);

        QJsonObject toJson() const;
        void fromJson(const QJsonObject &src, QStringList &errorList);

        void writeToFile(const QString &fileName) const;
        bool readFromFile(const QString &fileName);

        entity::SharedType type() const;
        void setType(const entity::SharedType &type);

        static QString typeIDMark();

    private:
        entity::SharedType m_Type;
        QString m_Description;
        Multiplicity m_Multiplicity;
    };

} // namespace relationship
