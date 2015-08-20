/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 16/08/2015.
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
#include "TestClassComponents.h"

#include "constants.h"

#define check_moving(type, name) \
    type name##1(*name);\
    type name##2(std::move(name##1));\
    ASSERT_EQ(*name, name##2);\
    type name##3 = std::move(name##2);\
    ASSERT_EQ(*name, name##3);

#define test_basic_prop(Name, name) \
    ASSERT_TRUE(!!property->add##Name().name()); \
    property->delete##Name(); \
    ASSERT_FALSE(!!property->name());

#define test_script_des(Name, name) \
    ASSERT_TRUE(property->is##Name()); \
    ASSERT_TRUE(property->is##Name##Default()); \
    ASSERT_TRUE(!!property->add##Name##Getter().name##Getter()); \
    property->delete##Name##Getter(); \
    ASSERT_FALSE(!!property->name##Getter()); \
    ASSERT_FALSE(property->set##Name(false).is##Name()); \

#define check_additional(Name) \
    ASSERT_TRUE(property->is##Name##Default()); \
    ASSERT_FALSE(property->set##Name(false).is##Name());

TEST_F(ClassComponents, Field)
{
    // Check a several methods which are not covered in previous tests
    auto field = std::make_shared<entity::Field>();
    ASSERT_TRUE(field->prefix().isEmpty());
    ASSERT_TRUE(field->suffix().isEmpty());
    ASSERT_FALSE(field->hasKeywords());

    field->setPrefix("m_");
    field->setSuffix("P");
    field->addKeyword(entity::FieldKeyword::Volatile);
    ASSERT_EQ(field->prefix(), "m_");
    ASSERT_EQ(field->suffix(), "P");
    ASSERT_TRUE(field->containsKeyword(entity::FieldKeyword::Volatile));

    // Check moving
    check_moving(entity::Field, field)
}

TEST_F(ClassComponents, Method)
{
    // Check a several methods which are not covered in previous tests
    auto method = std::make_shared<entity::ClassMethod>();
    ASSERT_FALSE(method->hasParameters());
    ASSERT_FALSE(method->hasLhsIdentificators());

    auto p1 = method->addParameter("p1", "stubID");
    method->addLhsIdentificator(entity::LhsIdentificator::Inline);
    ASSERT_TRUE(method->containsParameter(p1->name()));
    ASSERT_TRUE(method->containsLhsIdentficator(entity::LhsIdentificator::Inline));

    // Check moving
    check_moving(entity::ClassMethod, method)
}

TEST_F(ClassComponents, Property)
{
    // Check a several methods which are not covered in previous tests
    auto property = std::make_shared<entity::Property>();
    ASSERT_EQ(property->name(), DEFAULT_NAME);
    ASSERT_TRUE(!!property->field());
    ASSERT_EQ(property->name(), property->field()->name());

    test_basic_prop(Getter,   getter)
    test_basic_prop(Setter,   setter)
    test_basic_prop(Resetter, resetter)
    test_basic_prop(Notifier, notifier)
    test_basic_prop(Member,   member)

    ASSERT_TRUE(property->isRevisionDefault());
    ASSERT_EQ(10, property->setRevision(10).revision());

    test_script_des(Scriptable, scriptable)
    test_script_des(Designable, designable)

    check_additional(Stored)
    check_additional(User)
    check_additional(Constant)
    check_additional(Final)
    check_additional(Member)

    ASSERT_EQ(property->hashType(), entity::Property::staticHashType());
    ASSERT_EQ(property->marker(), entity::Property::staticMarker());

    property->setId("foo");
    ASSERT_EQ(property->id(), "foo");

    // Check moving
    check_moving(entity::Property, property)

    // Test additional class
    auto member = std::make_shared<entity::Member>();
    ASSERT_TRUE(member->isEmpty());

    member->name = "name";
    member->prefix = "prefix";
    member->suffix = "suffix";
    ASSERT_FALSE(member->isEmpty());

    auto secondMember = std::make_shared<entity::Member>();
    QStringList errors;
    secondMember->fromJson(member->toJson(), errors);
    ASSERT_TRUE(errors.isEmpty());
    ASSERT_EQ(*member, *secondMember);
}
