/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 04/07/2015.
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

#include "TestEntities.h"
#include "constants.h"

TEST_F(Enteties, SimpleType)
{
    ASSERT_STREQ(_type->name().toStdString().c_str(), BASE_TYPE_NAME);
    ASSERT_STREQ(_type->scopeId().toStdString().c_str(), GLOBAL_SCOPE_ID);

    _type->setName("some name");
    _type->setId("some id");
    _type->setScopeId("global scope id");

    // copy ctor
    auto newCopyType(std::make_unique<entity::Type>(*_type));
    ASSERT_EQ(*_type, *newCopyType);

    // copy operator
    newCopyType.reset();
    newCopyType = std::make_unique<entity::Type>();
    *newCopyType = *_type;
    ASSERT_EQ(*_type, *newCopyType);

    // move ctor
    entity::Type tmp(*_type);
    entity::Type moveType(std::move(tmp));
    ASSERT_TRUE(tmp.invalid());
    ASSERT_EQ(moveType, *_type);

    // move operator
    entity::Type tmpEq(*_type);
    entity::Type moveTypeEq;
    moveTypeEq = std::move(tmpEq);
    ASSERT_TRUE(tmpEq.invalid());
    ASSERT_EQ(moveTypeEq, *_type);
}

TEST_F(Enteties, ExtendedType)
{
}

TEST_F(Enteties, Class)
{
}

TEST_F(Enteties, Union)
{
}

TEST_F(Enteties, Enum)
{
}

TEST_F(Enteties, TemplateClass)
{
}
