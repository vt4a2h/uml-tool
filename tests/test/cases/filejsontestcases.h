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

TEST_F(FileJson, TypeJson)
{
    entity::SharedType type(std::make_shared<entity::Type>("stub_name", common::ID::nullID()));
    type->writeToFile(m_JsonFileName);

    auto type_comp(std::make_shared<entity::Type>());
    json_eq(type, type_comp, "Type")
}

TEST_F(FileJson, ExtendedTypeJson)
{
    entity::SharedExtendedType type =
        std::make_shared<entity::ExtendedType>("stub_name", common::ID::nullID());
    type->addLinkStatus();
    type->addPointerStatus(true);
    type->addTemplateParameter(common::ID::firstFreeID());
    type->addTemplateParameter(common::ID::firstFreeID().value() + 1);
    type->setConstStatus(true);
    type->setId(common::ID::firstFreeID().value() + 2);
    type->writeToFile(m_JsonFileName);

    auto type_comp(std::make_shared<entity::ExtendedType>());
    json_eq(type, type_comp, "ExtendedType")
}

TEST_F(FileJson, UnionJson)
{
    entity::SharedUnion union_ =
        std::make_shared<entity::Union>("stub_name", common::ID::nullID());
    union_->addField("stub_name", common::ID::nullID());
    union_->writeToFile(m_JsonFileName);

    auto union_comp(std::make_shared<entity::Union>());
    json_eq(union_, union_comp, "Union")
}

TEST_F(FileJson, EnumJson)
{
    entity::SharedEnum enum_(std::make_shared<entity::Enum>("stub_name", common::ID::nullID()));
    enum_->setStrongStatus(true);
    enum_->addElement("a");
    enum_->addElement("b");
    enum_->writeToFile(m_JsonFileName);

    auto enum_comp(std::make_shared<entity::Enum>());
    json_eq(enum_, enum_comp, "Enum")
}

TEST_F(FileJson, ClassJson)
{
    entity::SharedClass class_(std::make_shared<entity::Class>("stub_name", common::ID::nullID()));
    class_->addParent(common::ID::nullID(), entity::Protected);
    class_->addField("stub_field_name", common::ID::nullID());
    class_->addField("stub_field_name_1", common::ID::nullID());
    class_->makeMethod("stub_method_name");
    class_->makeMethod("stub_method_name_1");
    class_->writeToFile(m_JsonFileName);

    auto class_comp(std::make_shared<entity::Class>());
    json_eq(class_, class_comp, "Class");
}

TEST_F(FileJson, FieldJson)
{
    entity::SharedField field(std::make_shared<entity::Field>("stub_name", common::ID::nullID()));
    field->addKeyword(entity::Mutable);
    field->setPrefix("m_");
    field->setSection(entity::Private);
    field->setSuffix("foo");
    field->writeToFile(m_JsonFileName);

    auto field_comp(std::make_shared<entity::Field>());
    json_eq(field, field_comp, "Field");
}

TEST_F(FileJson, ClassMethodJson)
{
    entity::SharedMethod method(std::make_shared<entity::ClassMethod>("stub_name"));
    method->setConstStatus(true);
    method->setReturnTypeId(common::ID::nullID());
    method->setRhsIdentificator(entity::RhsIdentificator::Final);
    method->setSection(entity::Private);
    method->addLhsIdentificator(entity::LhsIdentificator::Inline);
    method->addParameter("stub_parameter", common::ID::nullID());
    method->addParameter("stub_parameter_1", common::ID::nullID());
    method->writeToFile(m_JsonFileName);

    auto method_comp(std::make_shared<entity::ClassMethod>());
    json_eq(method, method_comp, "ClassMethod")
}

TEST_F(FileJson, TemplateClassJson)
{
    auto class_(m_ProjectScope->addType<entity::TemplateClass>("stub_name"));
    class_->addTemplateParameter(common::ID::nullID(), common::ID::nullID());
    class_->addTemplateParameter(common::ID::nullID(), common::ID::nullID());
    EXPECT_TRUE(!!class_->addLocalType("type"))
            << "Locale type must be added.";
    EXPECT_TRUE(!!class_->addLocalType("type_1"))
            << "Locale type must be added.";
    class_->writeToFile(m_JsonFileName);

    // Remove class to avoid clash
    m_ProjectScope->removeType(class_->id());

    auto class_comp(m_ProjectScope->addType<entity::TemplateClass>("stub_name_1"));
    json_eq(class_, class_comp, "TemplateClass")
    ASSERT_TRUE(class_comp->readFromFile(m_JsonFileName));
    EXPECT_EQ(*class_, *class_comp);
}

TEST_F(FileJson, TemplateClassMethodJson)
{
    // Add stub type in order to set template database as additional scpe searcher
    auto class_(m_ProjectScope->addType<entity::TemplateClass>("stub_name"));

    auto method(class_->makeMethod<entity::TemplateClassMethod>("stub name"));
    EXPECT_TRUE(!!method->addLocalType("type"))
            << "Locale type must be added.";
    EXPECT_TRUE(!!method->addLocalType("type_1"))
            << "Locale type must be added.";
    method->writeToFile(m_JsonFileName);

    // Remove method to avoid clash
    class_->removeMethod(method);

    auto method_comp(class_->makeMethod<entity::TemplateClassMethod>("stub_name 1"));
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
    entity::SharedProperty property =
        std::make_shared<entity::Property>("stub_property_name", common::ID::nullID());
    property->setTypeSearcher(m_GlobalDb);

    // Test with optional methods
    property->addGetter().addSetter().addResetter().addNotifier();
    property->writeToFile(m_JsonFileName);

    auto actual(std::make_shared<entity::Property>());
    actual->setTypeSearcher(m_GlobalDb);
    json_eq(property, actual, "Property");

    // Test without optional methods
    property->addDesignableGetter().addScriptableGetter();
    property->writeToFile(m_JsonFileName);

    actual = std::make_shared<entity::Property>();
    actual->setTypeSearcher(m_GlobalDb);
    json_eq(property, actual, "Property");
}

TEST_F(FileJson, NodeJson)
{
    relationship::SharedNode node =
        std::make_shared<relationship::Node>(nullptr, relationship::ZeroOrInf);
    node->setDescription("stub_description");
    node->writeToFile(m_JsonFileName);

    auto node_comp(std::make_shared<relationship::Node>());
    json_eq(node, node_comp, "Node");
}

TEST_F(FileJson, BasicRelationJson)
{
    test_relation(Relation, [&](){ relation->setName("description"); })
}

TEST_F(FileJson, AssociationRelationJson)
{
    test_relation(Association, [&](){
        relation->setGetSetTypeId(common::ID::nullID());
        relation->setFieldTypeId(common::ID::nullID());
    })
}

TEST_F(FileJson, DependencyRelation)
{
    test_relation(Dependency, [&](){
        relation->setMethod(std::make_shared<entity::ClassMethod>("stub_name"));
    })
}

TEST_F(FileJson, GeneralizationRelation)
{
    test_relation(Generalization, [&](){
        relation->setSection(entity::Private);
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
        entity::MethodsList methods;
        methods << m_FirstClass->makeMethod("first_method")
                << m_SecondClass->makeMethod("second_method");

        relation->addMethods(methods);
    })
}
