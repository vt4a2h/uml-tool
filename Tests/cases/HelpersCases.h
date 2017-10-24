/*****************************************************************************
**
** Copyright (C) 2017 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 19.
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

#include <DB/Database.h>

#include "Constants.h"

TEST(DBPath, TestSplit)
{
    QString pathTemplate("/foo/bar/baz.%1");
    auto splittedPath = DB::Database::splitPath(pathTemplate.arg(DATABASE_FILE_EXTENTION));

    ASSERT_EQ(splittedPath.name, "baz");
    ASSERT_EQ(splittedPath.path, "/foo/bar");

    splittedPath = DB::Database::splitPath("fewfevdew.rbrb.wedw\\\\];[");
    ASSERT_EQ(splittedPath.name, "");
    ASSERT_EQ(splittedPath.path, "");
}

TEST(DBPath, TestJoin)
{
    auto path = DB::Database::mkPath("/foo/bar", "baz");

    ASSERT_EQ(path, QString("/foo/bar/baz.%1").arg(DATABASE_FILE_EXTENTION));
}
