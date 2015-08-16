/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 16/08/2015.
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
#include "TestClassComponents.h"

#define check_moving(type, name) \
    type name##1(*name);\
    auto name##2(std::move(name##1));\
    ASSERT_EQ(*name, name##2);\
    auto name##3 = std::move(name##2);\
    ASSERT_EQ(*name, name##3);\

TEST_F(ClassComponents, Field)
{
    // Check a several methods which are not covered in previous tests
    auto field = std::make_shared<entity::Field>();
    ASSERT_TRUE(field->prefix().isEmpty());
    ASSERT_TRUE(field->suffix().isEmpty());
    ASSERT_FALSE(field->hasKeywords());

    field->setPrefix("m_");
    field->setSuffix("P");
    field->addKeyword(entity::FieldKeyword::Volatile);
    ASSERT_EQ(field->prefix(), "m_");
    ASSERT_EQ(field->suffix(), "P");
    ASSERT_TRUE(field->containsKeyword(entity::FieldKeyword::Volatile));

    // Check moving
    check_moving(entity::Field, field)
}

TEST_F(ClassComponents, Method)
{
    // Check a several methods which are not covered in previous tests
    auto method = std::make_shared<entity::ClassMethod>();
    ASSERT_FALSE(method->hasParameters());
    ASSERT_FALSE(method->hasLhsIdentificators());

    auto p1 = method->addParameter("p1", "stubID");
    method->addLhsIdentificator(entity::LhsIdentificator::Inline);
    ASSERT_TRUE(method->containsParameter(p1->name()));
    ASSERT_TRUE(method->containsLhsIdentficator(entity::LhsIdentificator::Inline));

    // Check moving
    check_moving(entity::ClassMethod, method)
}
