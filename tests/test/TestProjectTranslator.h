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
#include <entity/extendedtype.h>
#include <entity/field.h>
#include <entity/union.h>
#include <entity/class.h>
#include <entity/templateclass.h>
#include <entity/templateclassmethod.h>
#include <entity/property.h>

#include <translation/projecttranslator.h>
#include <translation/translator_types.hpp>
#include <translation/code.h>

#include <enums.h>
#include <constants.h>

class ProjectTranslatorTest : public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
        _globalDb  = std::make_shared<db::Database>("Global");
        _projectDb = std::make_shared<db::ProjectDatabase>("Project");

        _translator = std::make_shared<translation::ProjectTranslator>(_globalDb, _projectDb);

        _globalScope  = _globalDb->addScope();
        _globalDb->removeScope(_globalScope->id());
        _globalScope->setId(GLOBAL_SCOPE_ID);
        _globalDb->addExistsScope(_globalScope);
        // TODO: implement auto changing in id db when scope id changed

        _projectScope = _projectDb->addScope("project_scope");

        _int = _globalScope->addType("int");
    }

    db::SharedDatabase _globalDb;
    db::SharedProjectDatabase _projectDb;

    translation::SharedTranslator _translator;

    entity::SharedScope _globalScope;
    entity::SharedScope _projectScope;

    entity::SharedType _int;
};
