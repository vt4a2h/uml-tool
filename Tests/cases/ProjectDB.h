/*****************************************************************************
**
** Copyright (C) 2019 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 18/1/2019.
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

#include <QtTest/QSignalSpy>

#include <Project/ProjectDB.hpp>
#include <Project/Project.h>

TEST(ProjectDBTest, AddProject_Success)
{
    Projects::ProjectDatabase db;

    auto added = db.addProject(std::make_shared<Projects::Project>("Foo", "Bar"));
    ASSERT_TRUE(added);
}

TEST(ProjectDBTest, AddProject_Fail)
{
    Projects::ProjectDatabase db;

    auto added = db.addProject(nullptr);
    ASSERT_FALSE(added);

    auto project = std::make_shared<Projects::Project>("Foo", "Bar");
    db.addProject(project);
    added = db.addProject(project);
    ASSERT_FALSE(added);
}

TEST(ProjectDBTest, RemoveProject_Success)
{
    Projects::ProjectDatabase db;

    auto project = std::make_shared<Projects::Project>("Foo", "Bar");
    db.addProject(project);

    auto removed = db.removeProject(project);
    ASSERT_TRUE(removed);
}

TEST(ProjectDBTest, RemoveProject_Fail)
{
    Projects::ProjectDatabase db;

    auto removed = db.removeProject(nullptr);
    ASSERT_FALSE(removed);

    removed = db.removeProject(std::make_shared<Projects::Project>("Foo", "Bar"));
    ASSERT_FALSE(removed);
}

TEST(ProjectDBTest, AddProject_SignalEmmited)
{
    Projects::ProjectDatabase db;
    QSignalSpy sp(&db, &Projects::ProjectDatabase::projectAdded);

    db.addProject(std::make_shared<Projects::Project>("Foo", "Bar"));
    ASSERT_EQ(sp.count(), 1);
}

TEST(ProjectDBTest, RemoveProject_SignalEmmited)
{
    Projects::ProjectDatabase db;
    QSignalSpy sp(&db, &Projects::ProjectDatabase::projectRemoved);

    auto project = std::make_shared<Projects::Project>("Foo", "Bar");
    db.addProject(project);

    db.removeProject(project);
    ASSERT_EQ(sp.count(), 1);
}
