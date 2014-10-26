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

#include <constants.cpp>

class RelationMaker : public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
        _globalDb  = std::make_shared<db::Database>("Global");
        _projectDb = std::make_shared<db::ProjectDatabase>("Project");

        _globalScope = _globalDb->addScope(GLOBAL_SCOPE_ID, "");

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
