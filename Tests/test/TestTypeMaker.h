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

#include <Entity/Type.h>
#include <Entity/Class.h>
#include <Entity/Enum.h>
#include <Entity/Union.h>
#include <Entity/Scope.h>
#include <Entity/ExtendedType.h>
#include <Entity/TemplateClass.h>
#include <enums.h>

class TypeMaker: public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
        _typeHashes << Entity::Type::staticHashType() << Entity::Class::staticHashType()
                    << Entity::Enum::staticHashType() << Entity::Union::staticHashType()
                    << Entity::TemplateClass::staticHashType() << Entity::ExtendedType::staticHashType();

        _scope = std::make_shared<Entity::Scope>("sc");
        _types << _scope->addType("Foo") << _scope->addType<Entity::Class>("Bar")
               << _scope->addType<Entity::Enum>("Baz") << _scope->addType<Entity::Union>("FooBar")
               << _scope->addType<Entity::TemplateClass>("FooBarBaz")
               << _scope->addType<Entity::ExtendedType>("FooBaz");
    }

    Entity::SharedScope _scope;
    QList< Entity::SharedType > _types;
    QList< size_t > _typeHashes;
};
