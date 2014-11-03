/*****************************************************************************
** 
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 29/10/2014.
**
** This file is part of Q-UML (UML tool for Qt).
** 
** Q-UML is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** 
** Q-UML is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.

** You should have received a copy of the GNU Lesser General Public License
** along with Q-UML.  If not, see <http://www.gnu.org/licenses/>. 
**
*****************************************************************************/

#pragma once

#include <QHash>

#include <gtest/gtest.h>

#include <entity/type.h>
#include <entity/class.h>
#include <entity/enum.h>
#include <entity/union.h>
#include <entity/scope.h>
#include <entity/extendedtype.h>
#include <entity/templateclass.h>
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
