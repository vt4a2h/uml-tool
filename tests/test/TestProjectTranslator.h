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

        _globalScope = _globalDb->addScope(GLOBAL_SCOPE_ID, "");
        // TODO: complete test
    }

    db::SharedDatabase _globalDb;
    db::SharedDatabase _projectDb;

    entity::SharedScope _globalScope;
    entity::SharedScope _projectScope;
    entity::SharedEnum _fooEnum;
};
