/*****************************************************************************
**
** Copyright (C) 2018 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 08 .
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

#include <Entity/Enum.h>

#include "TestSectionalTextConverter.hpp"

using namespace Testing;

TEST_F(SectionalTextConverter, EnumToStrting_Success)
{
    Entity::Enum e("Foo", Common::ID::projectScopeID());
    QString result = m_Converter->toString(e);
    ASSERT_EQ(result.toStdString(), "enum Foo\n");
    ASSERT_TRUE(m_Messenger->messages().empty());

    e.setStrongStatus(true);
    result = m_Converter->toString(e);
    ASSERT_EQ(result.toStdString(), "enum class Foo\n");
    ASSERT_TRUE(m_Messenger->messages().empty());

    e.setEnumTypeId(m_GlobalDb->typeByName("int")->id());
    result = m_Converter->toString(e);
    ASSERT_EQ(result.toStdString(), "enum class Foo int\n");

    e.addElement("element1")->setValue(2);
    e.addElement("element2")->setValue(5);
    result = m_Converter->toString(e);
    ASSERT_EQ(result.toStdString(), "enum class Foo int\n"
                                    "element1 2\n"
                                    "element2 5\n");
}

TEST_F(SectionalTextConverter, EnumToStrting_Fail)
{

}

TEST_F(SectionalTextConverter, EnumFromString)
{

}
