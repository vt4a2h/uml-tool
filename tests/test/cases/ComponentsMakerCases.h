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

#include "TestComponentsMaker.h"

#include <entity/field.h>
#include <entity/Property.h>

#include <utility/helpfunctions.h>

#include "Constants.h"

#define check_errors(r)\
    ASSERT_TRUE(r.errorMessage.isEmpty())\
        << "There are some message: " << r.errorMessage.toStdString();

#define tst_common(name_, match) \
    const auto &name_ = property->name_(); \
    ASSERT_TRUE(!!name_); \
    ASSERT_EQ(name_->name(), match);

namespace {
    auto to_f(const common::BasicElement *e){ return static_cast<const entity::Field*>(e); }
    auto to_p(const common::SharedBasicEntity &e){ return std::static_pointer_cast<entity::Property>(e); }
    auto to_m(const common::SharedBasicEntity &e){ return std::static_pointer_cast<entity::ClassMethod>(e); }
    auto to_et(const entity::Type *e){ return static_cast<const entity::ExtendedType*>(e); }
}

TEST_F(ComponentsMaker, MakingField)
{
    // type and name
    auto result = parseAndMake("int a", models::DisplayPart::Fields);
    check_errors(result)

    auto field = to_f(result.resultEntity.get());
    ASSERT_EQ(field->name(), "a");

    auto globalScope = m_GlobalDatabase->scope(common::ID::globalScopeID());
    auto t = globalScope->type(field->typeId());
    ASSERT_TRUE(!!t) << "Type with name int is not found in global database";
    ASSERT_EQ(t->name(), "int");

    // const and name
    result = parseAndMake("const int a", models::DisplayPart::Fields);
    check_errors(result)

    field = to_f(result.resultEntity.get());
    t = globalScope->type(field->typeId());
    ASSERT_FALSE(!!t) << "Type shouldn't be added to the global database.";
    t = m_Project->database()->typeByID(field->typeId());
    ASSERT_TRUE(!!t) << "Type should be added to the project database.";
    ASSERT_TRUE(to_et(t.get())->isConst()) << "Type should be const.";

    // const ptr to const
    result = parseAndMake("const int * const a", models::DisplayPart::Fields);
    check_errors(result)
    field = to_f(result.resultEntity.get());
    t = m_Project->database()->typeByID(field->typeId());
    const entity::ExtendedType::PlList& pl = to_et(t.get())->pl();
    ASSERT_FALSE(pl.isEmpty()) << "* const should be caught.";

    // static
    result = parseAndMake("static int a", models::DisplayPart::Fields);
    check_errors(result)
    field = to_f(result.resultEntity.get());
    const entity::FieldKeywordsList& lst = field->keywords();
    ASSERT_FALSE(lst.isEmpty());
    ASSERT_EQ(lst.count(), 1);
    ASSERT_EQ(lst[0], entity::FieldKeyword::FieldStatic) << "Filed should be static.";

    // std
    result = parseAndMake("std::unordered_set a", models::DisplayPart::Fields);
    check_errors(result)
    field = to_f(result.resultEntity.get());
    auto scopeStd = m_GlobalDatabase->chainScopeSearch(QStringList() << "std");
    ASSERT_TRUE(!!scopeStd) << "Scope std should be found.";
    t = m_GlobalDatabase->typeByID(field->typeId());
    ASSERT_TRUE(!!t) << "std::unordered_set should be placed in global database.";

    // with templates parameters
    result = parseAndMake("std::vector<int> v", models::DisplayPart::Fields);
    check_errors(result)
    field = to_f(result.resultEntity.get());
    auto vecOfInt = m_Project->database()->typeByID(field->typeId());
    ASSERT_TRUE(!!vecOfInt) << "Vector of int must be created in project database.";
}

TEST_F(ComponentsMaker, MakingMethod)
{
    // Type and name
    auto result = parseAndMake("int get()", models::DisplayPart::Methods);
    check_errors(result);

    auto method = to_m(result.resultEntity);
    ASSERT_EQ(method->name(), "get");

    auto globalScope = m_GlobalDatabase->scope(common::ID::globalScopeID());
    auto t = globalScope->type(method->returnTypeId());
    ASSERT_TRUE(!!t) << "Type with name int is not found in global database";
    ASSERT_EQ(t->name(), "int");

    // With arguments
    result = parseAndMake("int get(int index, double time)", models::DisplayPart::Methods);
    check_errors(result);

    method = to_m(result.resultEntity);
    ASSERT_TRUE(method->containsParameter("index"));
    ASSERT_TRUE(method->containsParameter("time"));

    auto parameters = method->parameters();
    t = globalScope->type(parameters[0]->typeId());
    ASSERT_EQ(t->name(), "int");
    ASSERT_TRUE(!!t);
    t = globalScope->type(parameters[1]->typeId());
    ASSERT_TRUE(!!t);
    ASSERT_EQ(t->name(), "double");

    // Lhs
    result = parseAndMake("friend int get()", models::DisplayPart::Methods);
    check_errors(result);

    method = to_m(result.resultEntity);
    ASSERT_TRUE(method->containsLhsIdentficator(entity::LhsIdentificator::Friend));

    // Const
    result = parseAndMake("friend int get() const", models::DisplayPart::Methods);
    check_errors(result);

    method = to_m(result.resultEntity);
    ASSERT_TRUE(method->isConst());

    // Rhs
    result = parseAndMake("virtual int get() const override", models::DisplayPart::Methods);
    check_errors(result);

    method = to_m(result.resultEntity);
    ASSERT_EQ(method->rhsIdentificator(), entity::RhsIdentificator::Override);
}

TEST_F(ComponentsMaker, MakingProperty)
{
    auto result = parseAndMake("int width", models::DisplayPart::Properties);
    check_errors(result);
    entity::SharedProperty property = to_p(result.resultEntity);

    // Name
    ASSERT_EQ(property->name(), "width");

    // Type
    auto globalScope = m_GlobalDatabase->scope(common::ID::globalScopeID());
    auto t = globalScope->type(property->typeId());
    ASSERT_TRUE(!!t) << "Type with name int is not found in global database";
    ASSERT_EQ(t->name(), "int");

    // Member
    result = parseAndMake("int width MEMBER m_width", models::DisplayPart::Properties);
    check_errors(result);
    property = to_p(result.resultEntity);
    ASSERT_TRUE(property->isMember());

    const auto &field = property->field();
    ASSERT_TRUE(!!field);
    ASSERT_EQ(field->name(), "width");
    ASSERT_EQ(field->prefix(), "m_");

    // Get/set/reset/notify
    result = parseAndMake("int width READ getWidth WRITE setWidth RESET clearWidth NOTIFY widthChanged",
                          models::DisplayPart::Properties);
    check_errors(result);
    property = to_p(result.resultEntity);
    tst_common(getter, "getWidth")
    tst_common(setter, "setWidth")
    tst_common(resetter, "clearWidth")
    tst_common(notifier, "widthChanged")

    // Revision
    result = parseAndMake("int width REVISION 10", models::DisplayPart::Properties);
    check_errors(result);
    property = to_p(result.resultEntity);
    ASSERT_EQ(property->revision(), 10);

    // Designable/scriptable
    result = parseAndMake("int width DESIGNABLE false SCRIPTABLE false", models::DisplayPart::Properties);
    check_errors(result);
    property = to_p(result.resultEntity);
    ASSERT_FALSE(property->isDesignable());
    ASSERT_FALSE(property->isScriptable());
    ASSERT_FALSE(!!property->designableGetter());
    ASSERT_FALSE(!!property->scriptableGetter());

    result = parseAndMake("int width DESIGNABLE isDesignable SCRIPTABLE isScriptable", models::DisplayPart::Properties);
    check_errors(result);
    property = to_p(result.resultEntity);
    ASSERT_TRUE(!!property->designableGetter());
    ASSERT_TRUE(!!property->scriptableGetter());
    ASSERT_EQ(property->designableGetter()->name(), "isDesignable");
    ASSERT_EQ(property->scriptableGetter()->name(), "isScriptable");

    // Stored/user/constant/final
    result = parseAndMake("int width STORED false USER true CONSTANT FINAL", models::DisplayPart::Properties);
    check_errors(result);
    property = to_p(result.resultEntity);
    ASSERT_FALSE(property->isStored());
    ASSERT_TRUE(property->isUser());
    ASSERT_TRUE(property->isConstant());
    ASSERT_TRUE(property->isFinal());

    // TODO: add some tests with negative result (e.g. wrong revision or smth like that)
}
