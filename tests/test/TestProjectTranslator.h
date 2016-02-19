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
#include <helpers/entityhelpres.h>

#include <project/project.h>
#include <project/project_types.hpp>

#include <helpers/generatorid.h>

#include <enums.h>
#include <constants.h>

class ProjectTranslatorTest : public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
        _p = std::make_shared<project::Project>();
        // Usually special slot is used for that
        const_cast<entity::GeneratorID&>(entity::GeneratorID::instance()).onCurrentProjectChanged(_p);
        _projectDb = _p->database();

        _globalDb = std::make_shared<db::Database>("Global");
        _p->setGlobalDatabase(_globalDb);

        _translator = std::make_shared<translation::ProjectTranslator>(_globalDb, _projectDb);

        _globalScope = std::make_shared<entity::Scope>();
        _globalScope->setId(entity::EntityID::globalScopeID());
        _globalDb->addExistsScope(_globalScope);

        _projectScope = _projectDb->getScope(entity::EntityID::projectScopeID());

        _int = std::make_shared<entity::Type>("int", _globalScope->id());
        _globalScope->addExistsType(_int);

        // TODO: load and use global database instead
        _void = _globalScope->addExistsType(
                    std::make_shared<entity::Type>("void", entity::EntityID::globalScopeID()));
    }

    project::SharedProject _p;

    db::SharedDatabase _globalDb;
    db::SharedProjectDatabase _projectDb;

    translation::SharedTranslator _translator;

    entity::SharedScope _globalScope;
    entity::SharedScope _projectScope;

    entity::SharedType _int;
    entity::SharedType _void;
};
