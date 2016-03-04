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

#include <entity/scope.h>
#include <entity/enum.h>
#include <entity/extendedtype.h>
#include <entity/field.h>
#include <entity/union.h>
#include <entity/class.h>
#include <entity/templateclass.h>
#include <entity/templateclassmethod.h>
#include <entity/property.h>

#include <translation/projecttranslator.h>
#include <translation/translator_types.hpp>
#include <translation/code.h>
#include <helpers/entityhelpres.h>

#include <helpers/generatorid.h>

#include <enums.h>
#include <constants.h>

#include "TestProjectBase.h"

class ProjectTranslatorTest : public ProjectBase
{
protected:
    virtual void SetUp() override
    {
        m_Translator = std::make_shared<translation::ProjectTranslator>(m_GlobalDb, m_ProjectDb);

        m_int = m_GlobalDb->typeByName("int");
        ASSERT_TRUE(!!m_int);

        m_void = m_GlobalDb->typeByName("void");
        ASSERT_TRUE(!!m_void);

        m_double = m_GlobalDb->typeByName("double");
        ASSERT_TRUE(!!m_double);

        m_qstring = m_GlobalDb->typeByName("QString");
        ASSERT_TRUE(!!m_void);
    }

    translation::SharedTranslator m_Translator;

    entity::SharedType m_int;
    entity::SharedType m_void;
    entity::SharedType m_double;
    entity::SharedType m_qstring;
};
