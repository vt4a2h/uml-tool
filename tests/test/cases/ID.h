/*****************************************************************************
**
** Copyright (C) 2017 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 23.
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

#include <gtest/gtest.h>

#include <common/ID.h>

TEST(IDTest, TestConstruction)
{
    using namespace common;

    ID id(10);

    ID id1(id);
    ASSERT_EQ(id1, id);

    ID id2;
    id2 = id;
    ASSERT_EQ(id, id2);

    ID id3(std::move(id));
    ASSERT_EQ(id3.value(), 10);

    ID id4;
    id4 = std::move(id3);
    ASSERT_EQ(id4.value(), 10);
}

TEST(IDTest, TestOperators)
{
    using namespace common;

    ID id(42);

    ASSERT_EQ(id + 1, ID(43));
    ASSERT_EQ(id + ID(1), ID(43));

    ID id2 = id;
    ID id3 = id2++;
    ASSERT_EQ(id2, id3 + 1);

    ASSERT_LT(id3, id2);
    ASSERT_LE(id3, id2);
    ASSERT_NE(id3, id2);
    ASSERT_GT(id2, id3);
    ASSERT_GE(id2, id3);
}

TEST(IDTest, TestCommonMethods)
{
    using namespace common;

    ID id;
    ASSERT_FALSE(id.isValid());
    ASSERT_EQ(id, ID::nullID());

    id = ID(42);
    ASSERT_EQ(id.toString(), QString::number(42));

    ASSERT_EQ(id.value(), 42);
    id.setValue(14);
    ASSERT_EQ(id.value(), 14);

    auto jsonID = id.toJson();
    ID id2;
    ErrorList el;
    id2.fromJson(jsonID, el);
    ASSERT_TRUE(el.empty());
    ASSERT_EQ(id, id2);
}
