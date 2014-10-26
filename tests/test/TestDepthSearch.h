#pragma once

#include <QHash>
#include <QString>

#include <gtest/gtest.h>

#include <entity/type.h>
#include <entity/class.h>
#include <entity/enum.h>
#include <entity/union.h>
#include <db/database.h>
#include <entity/scope.h>

class DepthSearch : public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
        _d = new db::Database();

        _scopes["sc1"] = _d->addScope("sc1");
        _scopes["sc2"] = _scopes["sc1"]->addChildScope("sc2");
        _scopes["sc3"] = _scopes["sc2"]->addChildScope("sc3");

        _scopes["sc4"] = _d->addScope("sc4");
        _scopes["sc5"] = _scopes["sc4"]->addChildScope("sc5");

        _types["Foo"] = _scopes["sc1"]->addType("Foo");
        _types["Bar"] = _scopes["sc2"]->addType<entity::Class>("Bar");
        _types["Baz"] = _scopes["sc3"]->addType<entity::Enum>("Baz");
        _types["FooBar"] = _scopes["sc4"]->addType<entity::Union>("FooBar");
    }

    virtual void TearDown() override
    {
        delete _d;
    }

    db::Database *_d;
    QHash<QString, entity::SharedScope> _scopes;
    QHash<QString, entity::SharedType> _types;
};
