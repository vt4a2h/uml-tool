/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 04/07/2015.
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

#include <entity/type.h>
#include <entity/extendedtype.h>
#include <entity/enum.h>
#include <entity/union.h>
#include <entity/class.h>
#include <entity/templateclass.h>
#include <entity/entity_types.hpp>

class Enteties : public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
        _type          = std::make_shared<entity::Type>()         ;
        _extendedType  = std::make_shared<entity::ExtendedType>() ;
        _enum          = std::make_shared<entity::Enum>()         ;
        _union         = std::make_shared<entity::Union>()        ;
        _class         = std::make_shared<entity::Class>()        ;
        _templateClass = std::make_shared<entity::TemplateClass>();
    }

    virtual void TearDown() override
    {
        _type.reset();
        _extendedType.reset();
        _enum.reset();
        _union.reset();
        _class.reset();
        _templateClass.reset();
    }

    entity::SharedType          _type         ;
    entity::SharedExtendedType  _extendedType ;
    entity::SharedEnum          _enum         ;
    entity::SharedUnion         _union        ;
    entity::SharedClass         _class        ;
    entity::SharedTemplateClass _templateClass;
};
