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

#include <entity/Type.h>
#include <entity/ExtendedType.h>
#include <entity/Enum.h>
#include <entity/Union.h>
#include <entity/Class.h>

class Components : public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
        _type = std::make_shared<entity::Type>();
        _extendedType = std::make_shared<entity::ExtendedType>();
        _enum = std::make_shared<entity::Enum>();
        _union = std::make_shared<entity::Union>();
        _class = std::make_shared<entity::Class>();
    }

    virtual void TearDown() override
    {
        _type.reset();
        _extendedType.reset();
        _enum.reset();
        _union.reset();
        _class.reset();
    }

    entity::SharedComponents _type;
    entity::SharedComponents _extendedType;
    entity::SharedComponents _enum;
    entity::SharedComponents _union;
    entity::SharedComponents _class;
};
