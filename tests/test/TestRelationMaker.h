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

#include <db/database.h>
#include <db/projectdatabase.h>
#include <entity/scope.h>
#include <entity/type.h>
#include <entity/class.h>
#include <entity/classmethod.h>
#include <entity/field.h>
#include <relationship/node.h>
#include <relationship/relation.h>
#include <relationship/generalization.h>
#include <relationship/dependency.h>
#include <relationship/association.h>
#include <relationship/multiplyassociation.h>
#include <relationship/realization.h>

#include <types.h>

#include <constants.h>

class RelationMaker : public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
        _globalDb  = std::make_shared<db::Database>("Global");
        _projectDb = std::make_shared<db::ProjectDatabase>("Project");

        _globalScope = std::make_shared<entity::Scope>();
        _globalScope->setId(entity::EntityID::globalScopeID());
        _globalDb->addExistsScope(_globalScope);

        _firstProjectScope  = _projectDb->addScope("First scope");
        _secondProjectScope = _projectDb->addScope("Second scope");

        _firstClass  = _firstProjectScope->addType<entity::Class>("First");
        _secondClass = _secondProjectScope->addType<entity::Class>("Second");
    }

    db::SharedDatabase _globalDb;
    db::SharedDatabase _projectDb;

    entity::SharedScope _globalScope;

    entity::SharedScope _firstProjectScope;
    entity::SharedScope _secondProjectScope;
    entity::SharedClass _firstClass;
    entity::SharedClass _secondClass;
};
