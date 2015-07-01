/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 01/07/2015.
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

#include "TestIComponents.h"

#define TEST_COMPONENTS(capName, pluralForm, capPluralForm) \
    void test##capPluralForm( entity::SharedComponents &components, bool contains ) \
    { \
        ASSERT_TRUE(components->pluralForm().empty()) \
                << "Methods should be empty first time."; \
\
        auto new##capName = components->addNew##capName(); \
        if (contains) \
            ASSERT_TRUE(new##capName.get()); \
        else \
            ASSERT_FALSE(new##capName.get()); \
\
        auto index = components->remove##capName(new##capName); \
        ASSERT_EQ(index, contains ? 0 : -1); \
\
        components->addExists##capName(new##capName, 0); \
        auto count = components->pluralForm().count(); \
        ASSERT_EQ(count, contains ? 1 : 0); \
    }

namespace {
    TEST_COMPONENTS(Method, methods, Methods)
    TEST_COMPONENTS(Field, fields, Fields)
    TEST_COMPONENTS(Element, elements, Elements)
    TEST_COMPONENTS(Property, properties, Properties)
}

TEST_F(Components, SimpleType)
{
    testMethods(_type, false);
    testFields(_type, false);
    testElements(_type, false);
    testProperties(_type, false);
}

TEST_F(Components, ExtendedType)
{
    testMethods(_extendedType, false);
    testFields(_extendedType, false);
    testElements(_extendedType, false);
    testProperties(_extendedType, false);
}

TEST_F(Components, Class)
{
    testMethods(_class, true);
    testFields(_class, true);
    testElements(_class, false);
    testProperties(_class, true);
}

TEST_F(Components, Union)
{
    testMethods(_union, false);
    testFields(_union, true);
    testElements(_union, false);
    testProperties(_union, false);
}

TEST_F(Components, Enum)
{
    testMethods(_enum, false);
    testFields(_enum, false);
    testElements(_enum, true);
    testProperties(_enum, false);
}
