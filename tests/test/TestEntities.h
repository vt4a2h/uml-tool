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

#include "TestProjectBase.h"

class Enteties : public ProjectBase
{
protected:
    virtual void SetUp() override
    {
        _type          = std::make_unique<entity::Type>()         ;
        _extendedType  = std::make_unique<entity::ExtendedType>() ;
        _enum          = std::make_unique<entity::Enum>()         ;
        _union         = std::make_unique<entity::Union>()        ;
        _class         = std::make_unique<entity::Class>()        ;
        _templateClass = std::make_unique<entity::TemplateClass>();
    }

    std::unique_ptr<entity::Type>          _type         ;
    std::unique_ptr<entity::ExtendedType>  _extendedType ;
    std::unique_ptr<entity::Enum>          _enum         ;
    std::unique_ptr<entity::Union>         _union        ;
    std::unique_ptr<entity::Class>         _class        ;
    std::unique_ptr<entity::TemplateClass> _templateClass;
};
