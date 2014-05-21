#pragma once

#include <gtest/gtest.h>

#include <projectdatabase.h>
#include <database.h>
#include <scope.h>
#include <enum.h>
#include <projecttranslator.h>
#include <types.h>

class Translator : public ::testing::Test
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
        _fooEnum = _projectScope->addType<entity::Enum>("Foo");
    }

    db::SharedDatabase _globalDb;
    db::SharedDatabase _projectDb;

    translator::SharedTranslator _translator;

    entity::SharedScope _globalScope;
    entity::SharedScope _projectScope;

    entity::SharedEnum  _fooEnum;
    entity::SharedType  _int;
};
