#pragma once

#include <QHash>

#include <gtest/gtest.h>

#include <type.h>
#include <class.h>
#include <enum.h>
#include <union.h>
#include <scope.h>
#include <extendedtype.h>
#include <templateclass.h>
#include <enums.h>

class TypeMaker: public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
        _typesNames << "Type"  << "Class" << "TemplateClass"
                    << "Union" << "Enum"  << "ExtendedType";

        _scope = std::make_shared<entity::Scope>("sc");
        _types["Type"]  = _scope->addType("Foo");
        _types["Class"] = _scope->addType<entity::Class>("Bar");
        _types["Enum"]  = _scope->addType<entity::Enum>("Baz");
        _types["Union"] = _scope->addType<entity::Union>("FooBar");
        _types["TemplateClass"] = _scope->addType<entity::TemplateClass>("FooBarBaz");
        _types["ExtendedType"]  = _scope->addType<entity::ExtendedType>("FooBaz");
    }

    entity::SharedScope _scope;
    QHash<QString, entity::SharedType> _types;
    QStringList _typesNames;
};
