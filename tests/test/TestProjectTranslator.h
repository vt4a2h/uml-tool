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
