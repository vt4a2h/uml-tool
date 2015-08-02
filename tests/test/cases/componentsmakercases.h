/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 13/07/2015.
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

#include "ComponentsMaker.h"

#include <entity/field.h>

#include "constants.h"

namespace {
    const QVector<QPair<QString, bool>> fieldData =
    {
        {"int ",                                          false},
        {"int a",                                         true },
        {"const int",                                     false},
        {"foo::bar::int a",                               true },
        {"const foo::bar::int a",                         true },
        {"constfoo::bar::int a",                          true },
        {"scons foo::bar::int a",                         false},
        {"constt foo::bar::int a",                        false},
        {"static const foo::bar::int a",                  true },
        {"staticconst foo::bar::int a",                   false},
        {"static const std::foo::baz::int * const **& a", true },
        {"int *consta",                                   true },
        {"std::vector<int> vec",                          true },
        {"std::vector<int, MyAlloc> vec",                 true },
        {"std::vector<int,> vec",                         true },
        {"std::vector<int><int> vec",                     false},
        {"std::vector< vec",                              false},
        {"std::vector> vec",                              false},
        {"std::vector<class, const> vec",                 false},
        {"std::vector<std::vector> vec",                  true },
    };

    auto to_f(const entity::BasicEntity *e){ return static_cast<const entity::Field*>(e); }
    auto to_et(const entity::Type *e){ return static_cast<const entity::ExtendedType*>(e); }
}

TEST_F(ComponentsMaker, CheckSignature)
{
    // Test for field
    for (auto &&testData: fieldData)
        ASSERT_EQ(m_Maker->signatureValid(testData.first, models::DisplayPart::Fields), testData.second)
            << "Signature: " << testData.first.toStdString().c_str();
}

TEST_F(ComponentsMaker, MakingField)
{
    // type and name
    auto result = m_Maker->makeComponent("int a", models::DisplayPart::Fields);
    ASSERT_TRUE(result.first.isEmpty()) << "There are some message: " << result.first.toStdString().c_str();

    auto field = to_f(result.second.get());
    ASSERT_EQ(field->name(), "a");

    auto globalScope = m_GlobalDatabase->getScope(GLOBAL);
    auto t = globalScope->getType(field->typeId());
    ASSERT_TRUE(!!t) << "Type with name int is not found in global database";
    ASSERT_EQ(t->name(), "int");

    // const and name
    result = m_Maker->makeComponent("const int a", models::DisplayPart::Fields);
    ASSERT_TRUE(result.first.isEmpty()) << "There are some message: " << result.first.toStdString().c_str();

    field = to_f(result.second.get());
    t = globalScope->getType(field->typeId());
    ASSERT_FALSE(!!t) << "Type shouldn't be added to the global database.";
    t = m_Project->database()->depthTypeSearch(field->typeId());
    ASSERT_TRUE(!!t) << "Type should be added to the project database.";
    ASSERT_TRUE(to_et(t.get())->isConst()) << "Type should be const.";

    // const ptr to const
    result = m_Maker->makeComponent("const int * const a", models::DisplayPart::Fields);
    ASSERT_TRUE(result.first.isEmpty()) << "There are some message: " << result.first.toStdString().c_str();
    field = to_f(result.second.get());
    t = m_Project->database()->depthTypeSearch(field->typeId());
    const entity::ExtendedType::PlList& pl = to_et(t.get())->pl();
    ASSERT_FALSE(pl.isEmpty()) << "* const should be caught.";

    // static
    result = m_Maker->makeComponent("static int a", models::DisplayPart::Fields);
    ASSERT_TRUE(result.first.isEmpty()) << "There are some message: " << result.first.toStdString().c_str();
    field = to_f(result.second.get());
    const entity::FieldKeywordsList& lst = field->keywords();
    ASSERT_FALSE(lst.isEmpty());
    ASSERT_EQ(lst.count(), 1);
    ASSERT_EQ(lst[0], entity::FieldKeyword::FieldStatic) << "Filed should be static.";

    // std
    result = m_Maker->makeComponent("std::unordered_set a", models::DisplayPart::Fields);
    ASSERT_TRUE(result.first.isEmpty()) << "There are some message: " << result.first.toStdString().c_str();
    field = to_f(result.second.get());
    auto scopeStd = m_GlobalDatabase->chainScopeSearch(QStringList() << "std");
    ASSERT_TRUE(!!scopeStd) << "Scope std should be found.";
    t = m_GlobalDatabase->depthTypeSearch(field->typeId());
    ASSERT_TRUE(!!t) << "std::unordered_set should be placed in global database.";

    // with templates parameters
    result = m_Maker->makeComponent("std::vector<int> v", models::DisplayPart::Fields);
    ASSERT_TRUE(result.first.isEmpty()) << "There are some message: " << result.first.toStdString().c_str();
    field = to_f(result.second.get());
    auto vecOfInt = m_Project->database()->depthTypeSearch(field->typeId());
    ASSERT_TRUE(!!vecOfInt) << "Vector of int must be created in project database.";
}
