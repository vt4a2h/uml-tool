/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 19/02/2016.
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

#include <Entity/EntityTypes.hpp>
#include <DB/DBTypes.hpp>

namespace DB {

    /// The type searcher class. Provides weak dependency for class Property.
    class ITypeSearcher
    {
    public:
        virtual ~ITypeSearcher() = default;

        virtual Entity::SharedType typeByID(const Common::ID &typeId) const = 0;
        virtual Entity::SharedType typeByName(const QString &name) const = 0;
    };

    inline bool operator ==(const WeakTypeSearcher &lhs, const WeakTypeSearcher &rhs)
    {
        return lhs.lock() == rhs.lock();
    }

    inline uint qHash(const SharedTypeSearcher &e) { return ::qHash(e.get());  }
    inline uint qHash(const WeakTypeSearcher &e)   { return   qHash(e.lock()); }

} // namespace db
