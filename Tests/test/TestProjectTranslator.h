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

#include <Entity/Scope.h>
#include <Entity/Enum.h>
#include <Entity/ExtendedType.h>
#include <Entity/field.h>
#include <Entity/Union.h>
#include <Entity/Class.h>
#include <Entity/TemplateClass.h>
#include <Entity/TemplateClassMethod.h>
#include <Entity/Property.h>

#include <Translation/projecttranslator.h>
#include <Translation/translator_types.hpp>
#include <Translation/code.h>
#include <Helpers/entityhelpres.h>

#include <Helpers/GeneratorID.h>

#include <enums.h>
#include <Constants.h>

#include "TestProjectBase.h"

class ProjectTranslatorTest : public ProjectBase
{
protected:
    virtual void SetUp() override
    {
        m_Translator = std::make_shared<Translation::ProjectTranslator>(m_GlobalDb, m_ProjectDb);

        m_int = m_GlobalDb->typeByName("int");
        ASSERT_TRUE(!!m_int);

        m_void = m_GlobalDb->typeByName("void");
        ASSERT_TRUE(!!m_void);

        m_double = m_GlobalDb->typeByName("double");
        ASSERT_TRUE(!!m_double);

        m_qstring = m_GlobalDb->typeByName("QString");
        ASSERT_TRUE(!!m_void);
    }

    Translation::SharedTranslator m_Translator;

    Entity::SharedType m_int;
    Entity::SharedType m_void;
    Entity::SharedType m_double;
    Entity::SharedType m_qstring;
};
