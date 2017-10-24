/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 28/06/2015.
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

#include "TestJson.h"

#include <Entity/TemplateClass.h>
#include <Entity/ExtendedType.h>

TEST_F(FileJson, TypeJson)
{
    Entity::SharedType type(std::make_shared<Entity::Type>("stub_name", Common::ID::nullID()));
    type->writeToFile(m_JsonFileName);

    auto type_comp(std::make_shared<Entity::Type>());
    json_eq(type, type_comp, "Type")
}

TEST_F(FileJson, ExtendedTypeJson)
{
    Entity::SharedExtendedType type =
        std::make_shared<Entity::ExtendedType>("stub_name", Common::ID::nullID());
    type->addLinkStatus();
    type->addPointerStatus(true);
    type->addTemplateParameter(Common::ID::firstFreeID());
    type->addTemplateParameter(Common::ID::firstFreeID() + 1);
    type->setConstStatus(true);
    type->setId(Common::ID::firstFreeID() + 2);
    type->writeToFile(m_JsonFileName);

    auto type_comp(std::make_shared<Entity::ExtendedType>());
    json_eq(type, type_comp, "ExtendedType")
}

TEST_F(FileJson, UnionJson)
{
    Entity::SharedUnion union_ =
        std::make_shared<Entity::Union>("stub_name", Common::ID::nullID());
    union_->addField("stub_name", Common::ID::nullID());
    union_->writeToFile(m_JsonFileName);

    auto union_comp(std::make_shared<Entity::Union>());
    json_eq(union_, union_comp, "Union")
}

TEST_F(FileJson, EnumJson)
{
    Entity::SharedEnum enum_(std::make_shared<Entity::Enum>("stub_name", Common::ID::nullID()));
    enum_->setStrongStatus(true);
    enum_->addElement("a");
    enum_->addElement("b");
    enum_->writeToFile(m_JsonFileName);

    auto enum_comp(std::make_shared<Entity::Enum>());
    json_eq(enum_, enum_comp, "Enum")
}

TEST_F(FileJson, ClassJson)
{
    Entity::SharedClass class_(std::make_shared<Entity::Class>("stub_name", Common::ID::nullID()));
    class_->addParent(Common::ID::nullID(), Entity::Protected);
    class_->addField("stub_field_name", Common::ID::nullID());
    class_->addField("stub_field_name_1", Common::ID::nullID());
    class_->makeMethod("stub_method_name");
    class_->makeMethod("stub_method_name_1");
    class_->writeToFile(m_JsonFileName);

    auto class_comp(std::make_shared<Entity::Class>());
    json_eq(class_, class_comp, "Class");
}

TEST_F(FileJson, FieldJson)
{
    Entity::SharedField field(std::make_shared<Entity::Field>("stub_name", Common::ID::nullID()));
    field->addKeyword(Entity::Mutable);
    field->setPrefix("m_");
    field->setSection(Entity::Private);
    field->setSuffix("foo");
    field->writeToFile(m_JsonFileName);

    auto field_comp(std::make_shared<Entity::Field>());
    json_eq(field, field_comp, "Field");
}

TEST_F(FileJson, ClassMethodJson)
{
    Entity::SharedMethod method(std::make_shared<Entity::ClassMethod>("stub_name"));
    method->setConstStatus(true);
    method->setReturnTypeId(Common::ID::nullID());
    method->setRhsIdentificator(Entity::RhsIdentificator::Final);
    method->setSection(Entity::Private);
    method->addLhsIdentificator(Entity::LhsIdentificator::Inline);
    method->addParameter("stub_parameter", Common::ID::nullID());
    method->addParameter("stub_parameter_1", Common::ID::nullID());
    method->writeToFile(m_JsonFileName);

    auto method_comp(std::make_shared<Entity::ClassMethod>());
    json_eq(method, method_comp, "ClassMethod")
}

TEST_F(FileJson, TemplateClassJson)
{
    auto class_(m_ProjectScope->addType<Entity::TemplateClass>("stub_name"));
    class_->addTemplateParameter(Common::ID::nullID(), Common::ID::nullID());
    class_->addTemplateParameter(Common::ID::nullID(), Common::ID::nullID());
    EXPECT_TRUE(!!class_->addLocalType("type"))
            << "Locale type must be added.";
    EXPECT_TRUE(!!class_->addLocalType("type_1"))
            << "Locale type must be added.";
    class_->writeToFile(m_JsonFileName);

    // Remove class to avoid clash
    m_ProjectScope->removeType(class_->id());

    auto class_comp(m_ProjectScope->addType<Entity::TemplateClass>("stub_name_1"));
    json_eq(class_, class_comp, "TemplateClass")
    ASSERT_TRUE(class_comp->readFromFile(m_JsonFileName));
    EXPECT_EQ(*class_, *class_comp);
}

TEST_F(FileJson, TemplateClassMethodJson)
{
    // Add stub type in order to set template database as additional scpe searcher
    auto class_(m_ProjectScope->addType<Entity::TemplateClass>("stub_name"));

    auto method(class_->makeMethod<Entity::TemplateClassMethod>("stub name"));
    EXPECT_TRUE(!!method->addLocalType("type"))
            << "Locale type must be added.";
    EXPECT_TRUE(!!method->addLocalType("type_1"))
            << "Locale type must be added.";
    method->writeToFile(m_JsonFileName);

    // Remove method to avoid clash
    class_->removeMethod(method);

    auto method_comp(class_->makeMethod<Entity::TemplateClassMethod>("stub_name 1"));
    json_eq(method, method_comp, "TemplateClassMethod");
}

TEST_F(FileJson, ScopeJson)
{
    auto scope = m_ProjectDb->addScope("stub_scope_name");
    scope->addType("type");
    scope->addType("type_1");
    scope->addChildScope("scope");
    scope->addChildScope("scope_1");
    scope->writeToFile(m_JsonFileName);

    // Remove scope to avoid clash
    m_ProjectDb->removeScope(scope->id());

    auto scope_comp = m_ProjectDb->addScope("another_scope_name");
    scope_comp->readFromFile(m_JsonFileName);
    json_eq(scope, scope_comp, "Scope");
}

TEST_F(FileJson, PropertyJson)
{
    Entity::SharedProperty property =
        std::make_shared<Entity::Property>("stub_property_name", Common::ID::nullID());
    property->setTypeSearcher(m_GlobalDb);

    // Test with optional methods
    property->addGetter().addSetter().addResetter().addNotifier();
    property->writeToFile(m_JsonFileName);

    auto actual(std::make_shared<Entity::Property>());
    actual->setTypeSearcher(m_GlobalDb);
    json_eq(property, actual, "Property");

    // Test without optional methods
    property->addDesignableGetter().addScriptableGetter();
    property->writeToFile(m_JsonFileName);

    actual = std::make_shared<Entity::Property>();
    actual->setTypeSearcher(m_GlobalDb);
    json_eq(property, actual, "Property");
}

TEST_F(FileJson, NodeJson)
{
    Relationship::SharedNode node =
        std::make_shared<Relationship::Node>(nullptr, Relationship::ZeroOrInf);
    node->setDescription("stub_description");
    node->writeToFile(m_JsonFileName);

    auto node_comp(std::make_shared<Relationship::Node>());
    json_eq(node, node_comp, "Node");
}

TEST_F(FileJson, BasicRelationJson)
{
    test_relation(Relation, [&](){ relation->setName("description"); })
}

TEST_F(FileJson, AssociationRelationJson)
{
    test_relation(Association, [&](){
        relation->setGetSetTypeId(Common::ID::nullID());
        relation->setFieldTypeId(Common::ID::nullID());
    })
}

TEST_F(FileJson, DependencyRelation)
{
    test_relation(Dependency, [&](){
        relation->setMethod(std::make_shared<Entity::ClassMethod>("stub_name"));
    })
}

TEST_F(FileJson, GeneralizationRelation)
{
    test_relation(Generalization, [&](){
        relation->setSection(Entity::Private);
    })
}

TEST_F(FileJson, MultiplyAssociationRelation)
{
    test_relation(MultiplyAssociation, [&](){
        auto key(m_GlobalScope->addType("key_stub"));
        auto container(m_ProjectScope->addType("container_stub"));

        relation->setContainerTypeId(container->id());
        relation->setKeyTypeId(key->id());
    })
}

TEST_F(FileJson, RealizationRelation)
{
    test_relation(Realization, [&](){
        Entity::MethodsList methods;
        methods << m_FirstClass->makeMethod("first_method")
                << m_SecondClass->makeMethod("second_method");

        relation->addMethods(methods);
    })
}
