#pragma once

#include <gtest/gtest.h>
#include <type.h>
#include <class.h>
#include <database.h>
#include <scope.h>

class TestDepthSearch : public ::testing::Test {
protected:
    virtual void SetUp() override
    {
        _d = new db::Database();

        sc1 = _d->addScope("sc1");
        sc2 = sc1->addChildScope("sc2");
        sc3 = sc2->addChildScope("sc3");

        sc4 = _d->addScope("sc4");
        sc5 = sc4->addChildScope("sc5");
    }

    virtual void TearDown() override
    {
        delete _d;
    }

    db::Database *_d;
    entity::SharedScope sc1;
    entity::SharedScope sc2;
    entity::SharedScope sc3;
    entity::SharedScope sc4;
    entity::SharedScope sc5;
};
