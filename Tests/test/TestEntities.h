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

#include <Entity/Type.h>
#include <Entity/ExtendedType.h>
#include <Entity/Enum.h>
#include <Entity/Union.h>
#include <Entity/Class.h>
#include <Entity/TemplateClass.h>
#include <Entity/entity_types.hpp>

#include "TestProjectBase.h"

class Enteties : public ProjectBase
{
protected:
    virtual void SetUp() override
    {
        _type          = std::make_unique<Entity::Type>()         ;
        _extendedType  = std::make_unique<Entity::ExtendedType>() ;
        _enum          = std::make_unique<Entity::Enum>()         ;
        _union         = std::make_unique<Entity::Union>()        ;
        _class         = std::make_unique<Entity::Class>()        ;
        _templateClass = std::make_unique<Entity::TemplateClass>();
    }

    std::unique_ptr<Entity::Type>          _type         ;
    std::unique_ptr<Entity::ExtendedType>  _extendedType ;
    std::unique_ptr<Entity::Enum>          _enum         ;
    std::unique_ptr<Entity::Union>         _union        ;
    std::unique_ptr<Entity::Class>         _class        ;
    std::unique_ptr<Entity::TemplateClass> _templateClass;
};
