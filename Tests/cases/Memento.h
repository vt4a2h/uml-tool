/*****************************************************************************
**
** Copyright (C) 20186 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 21/09/2018.
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

#include <Common/Memento.hpp>
#include <Common/BasicElement.h>

#include <Entity/Class.h>

TEST(MementoTest, SimpleConstruction)
{
    auto be = std::make_unique<Common::BasicElement>("Foo", Common::ID(42));

    Common::Memento memento(*be);

    ASSERT_EQ(memento.json(), be->toJson());
}

TEST(MementoTest, RestoreBasicElement_Success)
{
    auto be = std::make_unique<Common::BasicElement>("Foo", Common::ID(42));
    auto tmpBe = std::make_unique<Common::BasicElement>(*be);

    auto memento = be->exportState();
    ASSERT_TRUE(memento);

    be->setName("Foo1");

    ASSERT_NE(be->toJson(), memento->json());

    auto optErrs = be->importState(*memento);

    ASSERT_FALSE(optErrs.has_value());
    ASSERT_EQ(*be, *tmpBe);
}

namespace Common {

    class MementoTester
    {
    public:
        MementoTester(Memento &m) : m_m(m) {}

        QJsonObject &internalJson() { return m_m.m_JsonObj; }

    private:
        Memento &m_m;
    };

} // Common

TEST(MementoTest, RestoreBasicElement_Fail)
{
    auto be = std::make_unique<Common::BasicElement>("Foo", Common::ID(42));

    auto memento = be->exportState();
    ASSERT_TRUE(memento);

    auto &internalJson = Common::MementoTester(*memento).internalJson();
    internalJson.remove("Name");

    ASSERT_NE(be->toJson(), memento->json());

    auto optErrs = be->importState(*memento);

    ASSERT_TRUE(optErrs.has_value());
    ASSERT_NE(be->toJson(), memento->json());
}

static auto makeTstClass()
{
    auto cl = std::make_unique<Entity::Class>("Foo", Common::ID(42));

    cl->makeMethod("getFoo");
    cl->makeMethod("getBar");

    cl->addField("foo", Common::ID(42));
    cl->addField("bar", Common::ID(42));

    cl->addProperty("Foo", Common::ID(42));
    cl->addProperty("Bar", Common::ID(42));

    return cl;
}

TEST(MementoTest, RestoreClass_Success)
{
    auto cl = makeTstClass();
    auto tmpCl = std::make_shared<Entity::Class>(*cl);

    auto memento = cl->exportState();
    ASSERT_TRUE(memento);

    cl->setFinalStatus(true);
    ASSERT_NE(cl->exportState()->json(), memento->json());
    ASSERT_NE(*cl, *tmpCl);

    auto errors = cl->importState(*memento);
    ASSERT_FALSE(errors.has_value());
    ASSERT_EQ(cl->exportState()->json(), memento->json());
    ASSERT_EQ(*cl, *tmpCl);
}
