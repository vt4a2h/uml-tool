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

#include <DB/Database.h>
#include <DB/ProjectDatabase.h>
#include <Entity/Scope.h>
#include <Entity/Type.h>
#include <Entity/Class.h>
#include <Entity/ClassMethod.h>
#include <Entity/field.h>
#include <Relationship/node.h>
#include <Relationship/Relation.h>
#include <Relationship/generalization.h>
#include <Relationship/dependency.h>
#include <Relationship/association.h>
#include <Relationship/multiplyassociation.h>
#include <Relationship/realization.h>

#include <types.h>

#include <Constants.h>

#include "TestProjectBase.h"

class RelationMaker : public ProjectBase
{
protected:
    virtual void SetUp() override
    {
        m_FirstProjectScope  = m_ProjectDb->addScope("First scope");
        m_SecondProjectScope = m_ProjectDb->addScope("Second scope");

        m_FirstClass  = m_FirstProjectScope->addType<Entity::Class>("First");
        m_SecondClass = m_SecondProjectScope->addType<Entity::Class>("Second");
    }

    Entity::SharedScope m_FirstProjectScope;
    Entity::SharedScope m_SecondProjectScope;
    Entity::SharedClass m_FirstClass;
    Entity::SharedClass m_SecondClass;
};
