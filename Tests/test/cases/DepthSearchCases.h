/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 28/06/2015.
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

#include "TestDepthSearch.h"
#include "helpers.h"

TEST_F(DepthSearch, ScopeSearchWorks)
{
    Entity::SharedScope p(nullptr);

    search_circle(_scopes, scope, scope, true)
    invalid_case(scope, Common::ID::nullID(), true)
}

TEST_F(DepthSearch, TypeSearchWorks)
{
    Entity::SharedType p(nullptr);

    search_circle(_types, typeByID, type)
    invalid_case(typeByID, Common::ID::nullID())
}

TEST_F(DepthSearch, ChainScopeSearch)
{
    auto nullSc = _scopes["sc1"]->chainScopeSearch({});
    ASSERT_EQ(nullSc, nullptr)
            << "Null should be returned for empty scopes list.";

    auto sc2 = _scopes["sc1"]->chainScopeSearch({"sc2"});
    ASSERT_EQ(sc2, _scopes["sc2"])
            << "Scope sc2 should be exists: sc1::sc2";

    auto sc3 = _scopes["sc1"]->chainScopeSearch({"sc2", "sc3"});
    ASSERT_EQ(sc3, _scopes["sc3"])
            << "Scope sc3 should be exists: sc1::sc2::sc3.";

    sc3 = m_ProjectDb->chainScopeSearch({"sc1", "sc2", "sc3"});
    ASSERT_EQ(sc3, _scopes["sc3"])
            << "Scopes sc1 should be found in database: sc1::sc2::sc3.";
}
