#pragma once

#include <QHash>

#include <gtest/gtest.h>

#include <type.h>
#include <class.h>
#include <enum.h>
#include <union.h>
#include <scope.h>
#include <templateclass.h>
#include <enums.h>

class TestTypeMaker: public ::testing::Test {
protected:
    virtual void SetUp() override
    {
        _scope = std::make_shared<entity::Scope>("sc");
        _types["Type"]  = _scope->addType("Foo");
        _types["Class"] = _scope->addType<entity::Class>("Bar");
        _types["Enum"]  = _scope->addType<entity::Enum>("Baz");
        _types["Union"] = _scope->addType<entity::Union>("FooBar");
        _types["TemplateClass"] = _scope->addType<entity::TemplateClass>("FooBarBaz");
    }

    entity::SharedScope _scope;
    QHash<QString, entity::SharedType> _types;
};
