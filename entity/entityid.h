/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 31/01/2016.
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

#include <QtGlobal>

namespace entity
{

    // TODO: add tests
    /// ID for all entities
    class EntityID
    {
    public:
        EntityID();
        EntityID(quint64 value);
        EntityID(EntityID const&) = default;
        EntityID(EntityID &&) = default;

        EntityID &operator =(EntityID const&) = default;
        EntityID &operator =(EntityID &&) = default;

        friend bool operator ==(const EntityID &lhs, const EntityID &rhs);

        bool isValid() const;

        quint64 value() const;
        void setValue(const quint64 &value);

        // TODO: implement to/from JSON functions

    public: // Constants
        static EntityID nullID();

        static EntityID firstFreeID();
        static EntityID firstNonConstID();

        // Keep order please, add new item to the end. Next item value =
        // previous item value + 1, e.g. stdScopeID = globalScopeID + 1
        static EntityID globalScopeID();
        static EntityID stdScopeID();
        static EntityID globalDatabaseID();

    private:
        quint64 m_value;
    };

    uint qHash(const entity::EntityID &e);
}
