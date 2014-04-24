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
        _int    = _globalScope->addType("int");
        _double = _globalScope->addType("double");

        // TODO: cmpleted test
    }

    db::SharedDatabase _globalDb;
    db::SharedDatabase _projectDb;

    entity::SharedScope _globalScope;
    entity::SharedType _int;
    entity::SharedType _double;

    entity::Scope _projectScope;
};
