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

#include "TestRelationMaker.h"

TEST_F(RelationMaker, MultiplyAssociation)
{
    auto multAssociation =
            std::make_shared<relationship::MultiplyAssociation>(_firstClass->id(),
                                                                _secondClass->id(),
                                                                _globalDb.get(),
                                                                _projectDb.get());

    EXPECT_EQ(multAssociation->relationType(), relationship::MultiRelation)
            << "MultiplyAssociation should have type relationship::MultiRelation";

    auto valueType = _firstProjectScope->addType<entity::ExtendedType>("SharedSecond");
    auto stdScope  = _globalScope->addChildScope("std");
    auto sharedPtr = stdScope->addType("shared_ptr");
    valueType->setTypeId(sharedPtr->id());
    valueType->addTemplateParameter(_secondClass->id());

    auto containerType = _globalScope->addType<entity::ExtendedType>("SecondClasses");
    auto qhash = _globalScope->addType("QHash");
    containerType->setTypeId(qhash->id());
    auto stringClass = _globalScope->addType("QString");
    containerType->addTemplateParameter(stringClass->id());
    containerType->addTemplateParameter(valueType->id());

    multAssociation->setFieldTypeId(valueType->id());
    multAssociation->setGetSetTypeId(valueType->id());
    multAssociation->setContainerTypeId(containerType->id());
    multAssociation->setKeyTypeId(stringClass->id());

    multAssociation->makeRelation();

    EXPECT_TRUE(_firstClass->containsField(containerType->name()))
            << "First class should contain container with second classes";

    auto methods = _firstClass->methods();
    ASSERT_FALSE(methods.isEmpty())
            << "First class should have methods for work with container elemets";
    EXPECT_EQ(methods.count(), 4)
            << "First class should have 4 methods for work with container elemets";

    auto methodsList = _firstClass->getMethod("get" + _secondClass->name());
    ASSERT_FALSE(methodsList.isEmpty())
            << "First class should have a getter";
    EXPECT_TRUE(methodsList.first()->isConst())
            << "Getter shoudl be const";
    EXPECT_EQ(valueType->id(), methodsList.first()->returnTypeId())
            << "Getter should have valid return tyeid ";
    auto parameter = methodsList.first()->getParameter("key");
    ASSERT_NE(parameter, nullptr)
            << "Getter should have parameter with name \"key\"";
    EXPECT_EQ(parameter->typeId(), stringClass->id())
            << "Parameter in getter should have a valid type";

    methodsList = _firstClass->getMethod("add" + _secondClass->name());
    ASSERT_FALSE(methodsList.isEmpty())
            << "First class should have a setter";
    parameter = methodsList.first()->getParameter("src_" + _secondClass->name().toLower());
    ASSERT_NE(parameter, nullptr)
            << "Getter should have parameter with right name";
    EXPECT_EQ(parameter->typeId(), valueType->id())
            << "Parameter in setter should have a valid type";

    methodsList = _firstClass->getMethod("remove" + _secondClass->name());
    ASSERT_FALSE(methodsList.isEmpty())
            << "First class should have a deleter";
    parameter = methodsList.first()->getParameter("key");
    ASSERT_NE(parameter, nullptr)
            << "Deleter should have parameter with right name";
    EXPECT_EQ(parameter->typeId(), stringClass->id())
            << "Parameter in deleter should have a valid type";

    methodsList = _firstClass->getMethod(_secondClass->name().toLower() + "s");
    ASSERT_FALSE(methodsList.isEmpty())
            << "First class should have a group getter";
    EXPECT_TRUE(methodsList.first()->isConst())
            << "Group getter shoudl be const";
    EXPECT_EQ(containerType->id(), methodsList.first()->returnTypeId())
            << "Group getter should have valid return tyeid ";

    multAssociation->removeRelation();
    EXPECT_TRUE(_firstClass->methods().isEmpty() && _firstClass->fields().isEmpty())
            << "MultiplyAssociation should be removed";
}

TEST_F(RelationMaker, Association)
{
    auto association = std::make_shared<relationship::Association>(_firstClass->id(),
                                                                   _secondClass->id(),
                                                                   _globalDb.get(),
                                                                   _projectDb.get());

    EXPECT_EQ(association->relationType(), relationship::AssociationRelation)
            << "Association should have type relationship::Association";

    auto type = _firstProjectScope->addType<entity::ExtendedType>("PointerToSecondClass");
    type->setTypeId(_secondClass->id());
    type->addPointerStatus();

    association->setFieldTypeId(type->id());
    association->setGetSetTypeId(type->id());

    association->makeRelation();
    EXPECT_TRUE(_firstClass->containsField(_secondClass->name()))
            << "First class should contain second class type field";

    auto methodsList = _firstClass->getMethod(_secondClass->name().toLower());

    ASSERT_FALSE(methodsList.isEmpty()) << "First class should have a getter";
    EXPECT_TRUE(methodsList.first()->isConst()) << "Getter shoudl be const";
    EXPECT_EQ(type->id(), methodsList.first()->returnTypeId())
            << "Getter should have valid return tyeid ";

    methodsList = _firstClass->getMethod("set" + _secondClass->name());
    ASSERT_FALSE(methodsList.isEmpty()) << "First class should have a setter";
    auto parameters = methodsList.first()->parameters();
    ASSERT_FALSE(parameters.isEmpty()) << "Setter should have parameter";
    EXPECT_EQ(parameters.first()->name(), "src_" + _secondClass->name().toLower())
            << "Setter parameter should have valid name";
    EXPECT_EQ(parameters.first()->typeId(), type->id())
            << "Parameter in setter should have a valid type";

    association->removeRelation();
    EXPECT_TRUE(_firstClass->methods().isEmpty() && _firstClass->fields().isEmpty())
            << "Association should be removed";
}

TEST_F(RelationMaker, Generalization)
{
    auto generalization = std::make_shared<relationship::Generalization>(_firstClass->id(),
                                                                         _secondClass->id(),
                                                                         _globalDb.get(),
                                                                         _projectDb.get());

    EXPECT_EQ(generalization->relationType(), relationship::GeneralizationRelation)
            << "Generalization should have type relationship::GeneralizationRelation";

    generalization->makeRelation();
    EXPECT_TRUE(_firstClass->containsParent(_secondClass->id()))
            << "first class should inherit the second class";

    generalization->removeRelation();
    EXPECT_FALSE(_firstClass->containsParent(_secondClass->id()))
            << "relation should be removed";
}

TEST_F(RelationMaker, Dependency)
{
    auto ext = _globalScope->addType<entity::ExtendedType>("ConstLinkToSecondClass");
    ext->setTypeId(_secondClass->id());
    ext->addLinkStatus();
    ext->setConstStatus(true);

    auto depMethod = std::make_shared<entity::ClassMethod>("useSecond");
    depMethod->setConstStatus(true);
    depMethod->setReturnTypeId(entity::EntityID::voidID());
    depMethod->addParameter("second", ext->id());

    auto dependency = std::make_shared<relationship::Dependency>(_firstClass->id(),
                                                                 _secondClass->id(),
                                                                 _globalDb.get(),
                                                                 _projectDb.get());
    dependency->setMethod(depMethod);

    EXPECT_EQ(dependency->relationType(), relationship::DependencyRelation)
            << "Generalization should have type relationship::GeneralizationRelation";

    dependency->makeRelation();
    EXPECT_TRUE(_firstClass->containsMethod(depMethod->name()))
            << "first class should have method with second class id in parameters list";

    dependency->removeRelation();
    EXPECT_FALSE(_firstClass->containsMethod(depMethod->name()))
            << "relation should be removed";
}

// TODO: add tests for realization
