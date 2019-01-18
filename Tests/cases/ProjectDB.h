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

TEST(ProjectDBTest, AddProject_NormalProject_Fail)
{
    Projects::ProjectDatabase db;

    auto project = std::make_shared<Projects::Project>("Foo", "Bar");
    db.addProject(project);
    ASSERT_FALSE(db.addProject(project));
}

TEST(ProjectDBTest, AddProject_NullProject_Fail)
{
    Projects::ProjectDatabase db;

    auto added = db.addProject(nullptr);
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

TEST(ProjectDBTest, RemoveProject_NormalProject_Fail)
{
    Projects::ProjectDatabase db;

    auto removed = db.removeProject(std::make_shared<Projects::Project>("Foo", "Bar"));
    ASSERT_FALSE(removed);
}

TEST(ProjectDBTest, RemoveProject_NullProject_Fail)
{
    Projects::ProjectDatabase db;

    auto removed = db.removeProject(nullptr);
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

TEST(ProjectDBTest, ConvertToVector_InitialVectorIsEmpty)
{
    Projects::ProjectDatabase db;
    ASSERT_TRUE(db.projectsAsVector().isEmpty());
}

static auto genProjects()
{
    return QVector<Projects::SharedProject>{
        std::make_shared<Projects::Project>("Foo", "FooPath"),
        std::make_shared<Projects::Project>("Foo1", "Foo1Path"),
        std::make_shared<Projects::Project>("Foo2", "Foo2Path"),
        std::make_shared<Projects::Project>("Foo3", "Foo3Path"),
        std::make_shared<Projects::Project>("Foo4", "Foo4Path"),
    };
}

TEST(ProjectDBTest, ConvertToVector_VectorContainsAllProjects)
{
    Projects::ProjectDatabase db;

    auto projects = genProjects();
    for (auto &&p : projects)
        db.addProject(p);

    for (auto &&p : db.projectsAsVector())
        ASSERT_NE(projects.indexOf(p), -1);
}

TEST(ProjectDBTest, Contains_Success)
{
    Projects::ProjectDatabase db;

    db.addProject(std::make_shared<Projects::Project>("Foo", "FooPath"));
    ASSERT_TRUE(db.contains("Foo"));
}

TEST(ProjectDBTest, Contains_Fail)
{
    Projects::ProjectDatabase db;

    db.addProject(std::make_shared<Projects::Project>("Foo", "FooPath"));
    ASSERT_FALSE(db.contains("Bar"));
}

TEST(ProjectDBTest, IsEmpty_InitialState_Success)
{
    Projects::ProjectDatabase db;
    ASSERT_TRUE(db.isEmpty());
}


TEST(ProjectDBTest, IsEmpty_Success)
{
    Projects::ProjectDatabase db;

    auto p = std::make_shared<Projects::Project>("Foo", "FooPath");
    db.addProject(p);
    db.removeProject(p);
    ASSERT_TRUE(db.isEmpty());
}


TEST(ProjectDBTest, IsEmpty_Fail)
{
    Projects::ProjectDatabase db;

    db.addProject(std::make_shared<Projects::Project>("Foo", "FooPath"));
    ASSERT_FALSE(db.isEmpty());
}

TEST(ProjectDBTest, ProjectByName_Success)
{
    Projects::ProjectDatabase db;

    auto p = std::make_shared<Projects::Project>("Foo", "FooPath");
    db.addProject(p);

    auto foundP = db.projectByName("Foo");
    ASSERT_EQ(p, foundP);
}

TEST(ProjectDBTest, ProjectByName_Fail)
{
    Projects::ProjectDatabase db;

    db.addProject(std::make_shared<Projects::Project>("Foo", "FooPath"));
    ASSERT_FALSE(!!db.projectByName("Bar"));
}
