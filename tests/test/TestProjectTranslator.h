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

#include <db/projectdatabase.h>
#include <db/database.h>
#include <entity/scope.h>
#include <entity/enum.h>
#include <enums.h>
#include <entity/extendedtype.h>
#include <translator/projecttranslator.h>
#include <entity/templateclassmethod.h>
#include <types.h>
#include <translator/code.h>

class ProjectTranslatorTest : public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
        _globalDb  = std::make_shared<db::Database>("Global");
        _projectDb = std::make_shared<db::ProjectDatabase>("Project");

        _translator = std::make_shared<translator::ProjectTranslator>(_globalDb, _projectDb);

        _globalScope  = _globalDb->addScope();
        _projectScope = _projectDb->addScope("project_scope");

        _int = _globalScope->addType("int");
    }

    db::SharedDatabase _globalDb;
    db::SharedDatabase _projectDb;

    translator::SharedTranslator _translator;

    entity::SharedScope _globalScope;
    entity::SharedScope _projectScope;

    entity::SharedType _int;
};
