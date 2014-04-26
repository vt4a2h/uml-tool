#pragma once

#include <gtest/gtest.h>

#include <database.h>
#include <projectdatabase.h>
#include <scope.h>
#include <type.h>
#include <class.h>
#include <classmethod.h>
#include <field.h>
#include <node.h>

#include <relation.h>
#include <generalization.h>
#include <dependency.h>
#include <association.h>

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
