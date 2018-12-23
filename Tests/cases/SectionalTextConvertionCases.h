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

#include "Tests/TestSectionalTextConvertion.hpp"

using namespace Testing;

TEST_F(SectionalTextConvertion, EnumToStrting_Success)
{
    Entity::Enum e("Foo", Common::ID::projectScopeID());
    e.setTextConversionStrategy(m_EnumConversionStrategy);

    QString result = e.toString();
    ASSERT_EQ(result.toStdString(), "enum Foo\n");
    ASSERT_TRUE(m_Messenger->messages().empty());

    e.setStrongStatus(true);
    result = e.toString();
    ASSERT_EQ(result.toStdString(), "enum class Foo\n");
    ASSERT_TRUE(m_Messenger->messages().empty());

    e.setEnumTypeId(m_GlobalDb->typeByName("int")->id());
    result = e.toString();
    ASSERT_EQ(result.toStdString(), "enum class Foo int\n");

    e.addElement("element1")->setValue(2);
    e.addElement("element2")->setValue(5);
    result = e.toString();
    ASSERT_EQ(result.toStdString(), "enum class Foo int\n"
                                    "element1 2\n"
                                    "element2 5\n");

    e.removeEnumerator("element1");
    e.removeEnumerator("element2");
    ASSERT_TRUE(e.enumerators().isEmpty());

    e.addElement("e1");
    e.addElement("e2");
    result = e.toString();
    ASSERT_EQ(result.toStdString(), "enum class Foo int\n"
                                    "e1\n"
                                    "e2\n");
}

TEST_F(SectionalTextConvertion, EnumFromString_Base_Success)
{
    Entity::Enum enum_;
    enum_.setTextConversionStrategy(m_EnumConversionStrategy);

    enum_.fromString("enum Foo");

    ASSERT_EQ(m_Messenger->unreadMessagesCount(), 0);

    ASSERT_FALSE(enum_.isStrong());
    ASSERT_EQ(enum_.name().toStdString(), "Foo");
    ASSERT_EQ(enum_.enumTypeId(), Common::ID::nullID());
}

TEST_F(SectionalTextConvertion, EnumFromString_Scoped_Success)
{
    Entity::Enum enum_;
    enum_.setTextConversionStrategy(m_EnumConversionStrategy);

    enum_.fromString("enum class Foo int");

    ASSERT_EQ(m_Messenger->unreadMessagesCount(), 0);

    ASSERT_TRUE(enum_.isStrong());
    ASSERT_EQ(enum_.name().toStdString(), "Foo");

    auto type = Util::findType(enum_.enumTypeId(), m_GlobalDb, m_ProjectDb);
    ASSERT_TRUE(!!type);
    ASSERT_EQ(type->name().toStdString(), "int");
}

TEST_F(SectionalTextConvertion, EnumFromString_WithEnumerators_Success)
{
    static constexpr int enumeratorsCount = 3;
    const std::array<QString, enumeratorsCount> enumeratorNames = {"foo1", "foo2", "foo3"};

    QString enumStr = "enum class Foo int\n";
    for (auto &&en: enumeratorNames)
        enumStr += en + "\n";

    Entity::Enum enum_;
    enum_.setTextConversionStrategy(m_EnumConversionStrategy);

    enum_.fromString(enumStr);
    ASSERT_EQ(m_Messenger->unreadMessagesCount(), 0);

    auto enumerators = enum_.enumerators();
    ASSERT_EQ(enumerators.count(), enumeratorsCount);

    for (auto &&e: enumerators)
        ASSERT_FALSE(!!e->value());

    for (size_t i = 0; i < enumeratorNames.size(); ++i)
        ASSERT_EQ(enumeratorNames[i].toStdString(), enumerators[int(i)]->name().toStdString());
}

TEST_F(SectionalTextConvertion, EnumFromString_WithEnumeratorsAndValues_Success)
{
    static constexpr int enumeratorsCount = 3;
    const std::array<QString, enumeratorsCount> enumeratorNames = {"foo1 10", "foo2 015", "foo3 0xFF"};

    using Expected = std::tuple<QString, int, Entity::Enumerator::ValueBase>;
    std::array<Expected, enumeratorsCount> expected = {
        std::make_tuple(QString("foo1"), 10, Entity::Enumerator::Dec),
        std::make_tuple(QString("foo2"), 13, Entity::Enumerator::Oct),
        std::make_tuple(QString("foo3"), 255, Entity::Enumerator::Hex),
    };

    QString enumStr = "enum class Foo int\n";
    for (auto &&en: enumeratorNames)
        enumStr += en + "\n";

    Entity::Enum enum_;
    enum_.setTextConversionStrategy(m_EnumConversionStrategy);

    enum_.fromString(enumStr);
    ASSERT_EQ(m_Messenger->unreadMessagesCount(), 0);

    auto enumerators = enum_.enumerators();
    ASSERT_EQ(enumerators.count(), enumeratorsCount);

    for (auto &&e: enumerators)
        ASSERT_TRUE(!!e->value());

    for (size_t i = 0; i < enumeratorNames.size(); ++i) {
        auto const& [name, value, base] = expected[i];

        ASSERT_EQ(enumerators[i]->name().toStdString(), name.toStdString());
        ASSERT_EQ(enumerators[i]->value()->first, value);
        ASSERT_EQ(enumerators[i]->value()->second, base);
    }
}

TEST_F(SectionalTextConvertion, EnumFromString_Fail)
{
    Entity::Enum enum_;
    enum_.setTextConversionStrategy(m_EnumConversionStrategy);

    enum_.fromString("enum");
    ASSERT_TRUE(m_Messenger->unreadMessagesCount() > 0);
    m_Messenger->clear();

    enum_.fromString("enum ewfe efew");
    ASSERT_TRUE(m_Messenger->unreadMessagesCount() > 0);
    m_Messenger->clear();

    enum_.fromString("enum\n\r ewfe efew ewrw e\n");
    ASSERT_TRUE(m_Messenger->unreadMessagesCount() > 0);
    m_Messenger->clear();

    enum_.fromString("enum Foo\n32 a\ngwgew ewfew");
    ASSERT_TRUE(m_Messenger->unreadMessagesCount() > 0);
    m_Messenger->clear();
}
