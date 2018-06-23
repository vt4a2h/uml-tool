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

#include <Utility/helpfunctions.h>

#include "TestSectionalTextConverter.hpp"

using namespace Testing;

TEST_F(SectionalTextConverter, EnumToStrting_Success)
{
//    Entity::Enum e("Foo", Common::ID::projectScopeID());
//    QString result = m_Converter->toString(e);
//    ASSERT_EQ(result.toStdString(), "enum Foo\n");
//    ASSERT_TRUE(m_Messenger->messages().empty());

//    e.setStrongStatus(true);
//    result = m_Converter->toString(e);
//    ASSERT_EQ(result.toStdString(), "enum class Foo\n");
//    ASSERT_TRUE(m_Messenger->messages().empty());

//    e.setEnumTypeId(m_GlobalDb->typeByName("int")->id());
//    result = m_Converter->toString(e);
//    ASSERT_EQ(result.toStdString(), "enum class Foo int\n");

//    e.addElement("element1")->setValue(2);
//    e.addElement("element2")->setValue(5);
//    result = m_Converter->toString(e);
//    ASSERT_EQ(result.toStdString(), "enum class Foo int\n"
//                                    "element1 2\n"
//                                    "element2 5\n");

//    e.removeEnumerator("element1");
//    e.removeEnumerator("element2");
//    ASSERT_TRUE(e.enumerators().isEmpty());

//    e.addElement("e1");
//    e.addElement("e2");
//    result = m_Converter->toString(e);
//    ASSERT_EQ(result.toStdString(), "enum class Foo int\n"
//                                    "e1\n"
//                                    "e2\n");
}

TEST_F(SectionalTextConverter, EnumToStrting_Fail)
{

}

TEST_F(SectionalTextConverter, EnumFromString_Full_Success)
{
    Entity::Enum enum_;
    m_Converter->fromString("enum class Foo int", enum_);

    ASSERT_EQ(m_Messenger->unreadMessagesCount(), 0);

    ASSERT_TRUE(enum_.isStrong());
    ASSERT_EQ(enum_.name().toStdString(), "Foo");

    auto type = Util::findType(enum_.enumTypeId(), m_GlobalDb, m_ProjectDb);
    ASSERT_TRUE(!!type);
    ASSERT_EQ(type->name().toStdString(), "int");
}

TEST_F(SectionalTextConverter, EnumFromString_Base_Success)
{
    Entity::Enum enum_;
    m_Converter->fromString("enum Foo", enum_);

    ASSERT_EQ(m_Messenger->unreadMessagesCount(), 0);

    ASSERT_FALSE(enum_.isStrong());
    ASSERT_EQ(enum_.name().toStdString(), "Foo");
    ASSERT_EQ(enum_.enumTypeId(), Common::ID::nullID());
}

TEST_F(SectionalTextConverter, EnumFromString_Fail)
{
    Entity::Enum enum_;

    m_Converter->fromString("enum", enum_);
    ASSERT_TRUE(m_Messenger->unreadMessagesCount() > 0);
    m_Messenger->clear();

    m_Converter->fromString("enum ewfe efew", enum_);
    ASSERT_TRUE(m_Messenger->unreadMessagesCount() > 0);
    m_Messenger->clear();

    m_Converter->fromString("enum\n\r ewfe efew ewrw e\n", enum_);
    ASSERT_TRUE(m_Messenger->unreadMessagesCount() > 0);
    m_Messenger->clear();
}
