/*****************************************************************************
**
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 29/10/2014.
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

#include <types.h>

#include <db/database.h>
#include <generator/virtualdirectory.h>
#include <generator/basiccppprojectgenerator.h>
#include <helpers/entityhelpres.h>

class ProjectMaker : public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
        rootPath_.append(sep_).append("tmp");
        QDir().mkpath(rootPath_);

        generator_ = std::make_shared<generator::BasicCppProjectGenerator>(globalDb_,
                                                                           projectDb_,
                                                                           rootPath_);
        generator_->setProjectName("test_app");

        globalDb_->removeScope(globalScope_->id());
        globalScope_->setId(entity::EntityID::globalScopeID());
        globalDb_->addExistsScope(globalScope_);
    }

    virtual void TearDown() override
    {
        EXPECT_TRUE(QDir(rootPath_).removeRecursively());
    }

    db::SharedDatabase globalDb_  = std::make_shared<db::Database>("Global");
    db::SharedProjectDatabase projectDb_ = std::make_shared<db::ProjectDatabase>("Project");

    entity::SharedScope globalScope_   = globalDb_->addScope();

    entity::SharedScope standartScope_ = globalDb_->addScope("std");

    entity::SharedType int_     = globalScope_->addType("int");
    entity::SharedType bool_    = globalScope_->addExistsType(
        std::make_shared<entity::Type>("bool", entity::EntityID::globalScopeID(),
                                       entity::EntityID::firstFreeID().value() + 1)
    );
    entity::SharedType double_  = globalScope_->addType("double");
    entity::SharedType uint_    = globalScope_->addType("uint");
    entity::SharedType void_    = globalScope_->addExistsType(
        std::make_shared<entity::Type>("void", entity::EntityID::globalScopeID(),
                                       entity::EntityID::firstFreeID().value() + 2)
    );
    entity::SharedType nullptr_ = globalScope_->addType("nullptr");
    entity::SharedType string_  = standartScope_->addType("string");

    generator::SharedGenerator generator_;

    // file section
    QString rootPath_     = QDir::current().path();
    QChar   sep_          = QDir(rootPath_).separator();
    QString testDataPath_ = QString("..%1test_data%1").arg(sep_
                                                           );
};
