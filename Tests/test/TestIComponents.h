/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 01/07/2015.
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

#include <gtest/gtest.h>

#include <Entity/Type.h>
#include <Entity/ExtendedType.h>
#include <Entity/Enum.h>
#include <Entity/Union.h>
#include <Entity/Class.h>

class ComponentsTest : public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
        _type = std::make_shared<Entity::Type>();
        _extendedType = std::make_shared<Entity::ExtendedType>();
        _enum = std::make_shared<Entity::Enum>();
        _union = std::make_shared<Entity::Union>();
        _class = std::make_shared<Entity::Class>();
    }

    virtual void TearDown() override
    {
        _type.reset();
        _extendedType.reset();
        _enum.reset();
        _union.reset();
        _class.reset();
    }

    Entity::SharedComponents _type;
    Entity::SharedComponents _extendedType;
    Entity::SharedComponents _enum;
    Entity::SharedComponents _union;
    Entity::SharedComponents _class;
};
