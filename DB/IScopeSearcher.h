/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 29/05/2016.
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

#include <Common/CommonTypes.hpp>
#include <Entity/EntityTypes.hpp>
#include <DB/DBTypes.hpp>

namespace DB {

    /// Auxiliary class for looking up scopes
    class IScopeSearcher
    {
    public:
        virtual ~IScopeSearcher() = default;

        /// Search scope
        virtual Entity::SharedScope scope(const Common::ID &id, bool searchInDepth = false) const = 0;

        /// All scopes from the first nesting level
        virtual Entity::ScopesList scopes() const = 0;
    };

    inline bool operator ==(const WeakScopeSearcher &lhs, const WeakScopeSearcher &rhs)
    {
        return lhs.lock() == rhs.lock();
    }

} // namespace db
