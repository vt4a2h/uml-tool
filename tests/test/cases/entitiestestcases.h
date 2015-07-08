/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 04/07/2015.
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

#include "TestEntities.h"
#include "constants.h"

#define test_copy_move(entType, obj) \
    auto newCopyType(std::make_unique<entity::entType>(*obj)); \
    ASSERT_EQ(*obj, *newCopyType); \
    \
    newCopyType.reset(); \
    newCopyType = std::make_unique<entity::entType>();\
    *newCopyType = *obj;\
    ASSERT_EQ(*obj, *newCopyType);\
    \
    entity::entType tmp(*obj); \
    entity::entType moveType(std::move(tmp)); \
    ASSERT_EQ(moveType, *obj); \
    \
    entity::entType tmpEq(*obj); \
    entity::entType moveTypeEq; \
    moveTypeEq = std::move(tmpEq); \
    ASSERT_EQ(moveTypeEq, *obj); \

TEST_F(Enteties, SimpleType)
{
    ASSERT_STREQ(_type->name().toStdString().c_str(), BASE_TYPE_NAME);
    ASSERT_STREQ(_type->scopeId().toStdString().c_str(), GLOBAL_SCOPE_ID);

    _type->setName("some name");
    _type->setId("some id");
    _type->setScopeId("global scope id");

    test_copy_move(Type, _type);
}

TEST_F(Enteties, ExtendedType)
{
    ASSERT_STREQ(_extendedType->name().toStdString().c_str(), BASE_TYPE_NAME);
    ASSERT_STREQ(_extendedType->typeId().toStdString().c_str(), STUB_ID);
    ASSERT_FALSE(_extendedType->isConst());

    ASSERT_FALSE(_extendedType->isLink());
    _extendedType->addLinkStatus();
    ASSERT_TRUE(_extendedType->isLink());
    _extendedType->removeLinkStatus();
    ASSERT_FALSE(_extendedType->isLink());

    ASSERT_FALSE(_extendedType->isPointer());
    _extendedType->addPointerStatus();
    ASSERT_TRUE(_extendedType->isPointer());
    _extendedType->removePointerStatus();
    ASSERT_FALSE(_extendedType->isPointer());

    _extendedType->addPointerStatus();
    _extendedType->addLinkStatus();
    ASSERT_TRUE(_extendedType->isLink());
    ASSERT_TRUE(_extendedType->isPointer());

    _extendedType->setConstStatus(true);
    ASSERT_TRUE(_extendedType->isConst());
    _extendedType->setConstStatus(false);
    ASSERT_FALSE(_extendedType->isConst());

    const QString par1 = "par1";
    const QString par2 = "par2";

    _extendedType->addTemplateParameter(par1);
    ASSERT_TRUE(_extendedType->containsTemplateParameter(par1));
    _extendedType->addTemplateParameter(par2);
    ASSERT_TRUE(_extendedType->containsTemplateParameter(par2));

    _extendedType->removeTemplateParameters(par1);
    _extendedType->removeTemplateParameters(par2);

    ASSERT_TRUE(_extendedType->templateParameters().isEmpty());

    // Re-add one parameter to test copy and move
    _extendedType->addTemplateParameter(par1);

    test_copy_move(ExtendedType, _extendedType);
}

TEST_F(Enteties, Class)
{
    ASSERT_FALSE(_class->isFinal());
    ASSERT_EQ(_class->kind(), entity::ClassType);

    // Parents test
    const QString parentID = "stub_id";
    auto parent = _class->addParent(parentID, entity::Private);
    auto parents = _class->parents();

    ASSERT_FALSE(parents.isEmpty());
    ASSERT_TRUE(_class->containsParent(parentID));
    ASSERT_TRUE(_class->anyParents());
    ASSERT_EQ(parents.count(), 1);
    ASSERT_EQ(parent, parents.first());
    ASSERT_EQ(parent, _class->parent(parentID));

    _class->removeParent(parentID);
    ASSERT_TRUE(_class->parents().isEmpty());

    _class->addParent(parentID, entity::Private);
    _class->addParent(parentID + "42", entity::Private);
    ASSERT_EQ(_class->parents().count(), 2);

    // Methods test (some parts covered in IComponents tests)
    auto method = _class->makeMethod("Simple method");
    auto templateMethod = _class->makeMethod<entity::TemplateClassMethod>("Template method");
    ASSERT_EQ(method->hashType(), entity::ClassMethod::staticHashType());
    ASSERT_EQ(templateMethod->hashType(), entity::TemplateClassMethod::staticHashType());
    ASSERT_EQ(method, _class->getMethod(method->name()).first());
    ASSERT_EQ(templateMethod, _class->getMethod(templateMethod->name()).first());
    ASSERT_TRUE(_class->containsMethod(method->name()));
    ASSERT_TRUE(_class->containsMethod(templateMethod->name()));
    ASSERT_TRUE(_class->anyMethods());

    _class->removeMethods(method->name());
    ASSERT_FALSE(_class->containsMethod(method->name()));
    _class->addExistsMethod(method);

    ASSERT_TRUE(_class->containsMethods(entity::Public));
    ASSERT_EQ(_class->methods(entity::Public).count(), 2);

    method->setSection(entity::Private);
    templateMethod->setSection(entity::Protected);

    ASSERT_FALSE(_class->containsMethods(entity::Public));
    ASSERT_EQ(_class->methods(entity::Public).count(), 0);

    ASSERT_TRUE(_class->containsMethods(entity::Private));
    ASSERT_EQ(_class->methods(entity::Private).count(), 1);
    ASSERT_TRUE(_class->containsMethods(entity::Protected));
    ASSERT_EQ(_class->methods(entity::Protected).count(), 1);

    // Fields
    ASSERT_FALSE(_class->anyFields());
    auto field = _class->addField("Some field", "some_id");
    ASSERT_TRUE(_class->anyFields());
    ASSERT_TRUE(_class->containsField(field->name()));
    ASSERT_EQ(_class->getField(field->name()), field);
    _class->removeField(field->name());
    ASSERT_FALSE(_class->anyFields());

    ASSERT_FALSE(_class->containsFields(entity::Public));
    ASSERT_FALSE(_class->containsFields(entity::Private));
    ASSERT_FALSE(_class->containsFields(entity::Protected));
    _class->addExistsField(field);
    ASSERT_TRUE(_class->containsFields(entity::Public));
    field->setSection(entity::Private);
    ASSERT_TRUE(_class->containsFields(entity::Private));
    field->setSection(entity::Protected);
    ASSERT_TRUE(_class->containsFields(entity::Protected));

    // Properties
    ASSERT_FALSE(_class->anyProperties());
    auto property = _class->addProperty("some name", "some_id");
    ASSERT_TRUE(_class->anyProperties());
    ASSERT_EQ(property, _class->property(property->name()));
    ASSERT_TRUE(_class->containsProperty(property->name()));
    ASSERT_FALSE(_class->containsProperty("foobar"));
    _class->removeProperty(property->name());
    ASSERT_FALSE(_class->anyProperties());
    _class->addExistsProperty(property);

    test_copy_move(Class, _class)
}

TEST_F(Enteties, Union)
{
    test_copy_move(Union, _union)
}

TEST_F(Enteties, Enum)
{
    test_copy_move(Enum, _enum)
}

TEST_F(Enteties, TemplateClass)
{
    test_copy_move(TemplateClass, _templateClass)
}
