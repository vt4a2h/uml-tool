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

#include "Tests/TestEntities.h"

#include <boost/range/algorithm/equal.hpp>

#include <Entity/Property.h>

#include "Constants.h"

#define test_copy_move(entType, obj) \
    auto newCopyType(std::make_unique<Entity::entType>(*obj)); \
    ASSERT_EQ(*obj, *newCopyType); \
    \
    newCopyType.reset(); \
    newCopyType = std::make_unique<Entity::entType>();\
    *newCopyType = *obj;\
    ASSERT_EQ(*obj, *newCopyType);\
    \
    Entity::entType tmp(*obj); \
    Entity::entType moveType(std::move(tmp)); \
    ASSERT_EQ(moveType, *obj); \
    \
    Entity::entType tmpEq(*obj); \
    Entity::entType moveTypeEq; \
    moveTypeEq = std::move(tmpEq); \
    ASSERT_EQ(moveTypeEq, *obj); \

TEST_F(Enteties, SimpleType)
{
    ASSERT_STREQ(_type->name().toStdString().c_str(), BASE_TYPE_NAME.toStdString().c_str());
    ASSERT_EQ(_type->scopeId(), Common::ID::globalScopeID());

    _type->setName("some name");
    _type->setId(Common::ID::firstFreeID() + 1);
    _type->setScopeId(Common::ID::globalScopeID());

    test_copy_move(Type, _type)
}

TEST_F(Enteties, ExtendedType)
{
    ASSERT_STREQ(_extendedType->name().toStdString().c_str(), "Alias");
    ASSERT_EQ(_extendedType->typeId(), Common::ID::nullID());
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

    const Common::ID par1 = Common::ID::firstFreeID() + 1;
    const Common::ID par2 = par1 + 1;

    _extendedType->addTemplateParameter(par1);
    ASSERT_TRUE(_extendedType->containsTemplateParameter(par1));
    _extendedType->addTemplateParameter(par2);
    ASSERT_TRUE(_extendedType->containsTemplateParameter(par2));

    _extendedType->removeTemplateParameters(par1);
    _extendedType->removeTemplateParameters(par2);

    ASSERT_TRUE(_extendedType->templateParameters().isEmpty());

    // Re-add one parameter to test copy and move
    _extendedType->addTemplateParameter(par1);

    test_copy_move(ExtendedType, _extendedType)
}

TEST_F(Enteties, Class)
{
    ASSERT_FALSE(_class->isFinal());
    ASSERT_EQ(_class->kind(), Entity::ClassType);

    // Parents test
    const Common::ID parentID = Common::ID::firstFreeID() + 1;
    auto parent = _class->addParent(parentID, Entity::Private);
    auto parents = _class->parents();

    ASSERT_FALSE(parents.isEmpty());
    ASSERT_TRUE(_class->containsParent(parentID));
    ASSERT_TRUE(_class->anyParents());
    ASSERT_EQ(parents.count(), 1);
    ASSERT_EQ(parent, parents.first());
    ASSERT_EQ(parent, _class->parent(parentID));

    _class->removeParent(parentID);
    ASSERT_TRUE(_class->parents().isEmpty());

    _class->addParent(parentID, Entity::Private);
    _class->addParent(parentID + 42, Entity::Private);
    ASSERT_EQ(_class->parents().count(), 2);

    // Methods test (some parts covered in IComponents tests)
    auto method = _class->makeMethod("Simple method");
    auto templateMethod = _class->makeMethod<Entity::TemplateClassMethod>("Template method");
    ASSERT_EQ(method->hashType(), Entity::ClassMethod::staticHashType());
    ASSERT_EQ(templateMethod->hashType(), Entity::TemplateClassMethod::staticHashType());
    ASSERT_EQ(method, _class->getMethod(method->name()).first());
    ASSERT_EQ(templateMethod, _class->getMethod(templateMethod->name()).first());
    ASSERT_TRUE(_class->containsMethod(method->name()));
    ASSERT_TRUE(_class->containsMethod(templateMethod->name()));
    ASSERT_TRUE(_class->anyMethods());

    _class->removeMethods(method->name());
    ASSERT_FALSE(_class->containsMethod(method->name()));
    _class->addExistsMethod(method);

    ASSERT_TRUE(_class->containsMethods(Entity::Public));
    ASSERT_EQ(_class->methods(Entity::Public).count(), 2);

    method->setSection(Entity::Private);
    templateMethod->setSection(Entity::Protected);

    ASSERT_FALSE(_class->containsMethods(Entity::Public));
    ASSERT_EQ(_class->methods(Entity::Public).count(), 0);

    ASSERT_TRUE(_class->containsMethods(Entity::Private));
    ASSERT_EQ(_class->methods(Entity::Private).count(), 1);
    ASSERT_TRUE(_class->containsMethods(Entity::Protected));
    ASSERT_EQ(_class->methods(Entity::Protected).count(), 1);

    // Fields
    ASSERT_FALSE(_class->anyFields());
    auto field = _class->addField("Some field", Common::ID::nullID());
    ASSERT_TRUE(_class->anyFields());
    ASSERT_TRUE(_class->containsField(field->name()));
    ASSERT_EQ(_class->getField(field->name()), field);
    _class->removeField(field->name());
    ASSERT_FALSE(_class->anyFields());

    ASSERT_FALSE(_class->containsFields(Entity::Public));
    ASSERT_FALSE(_class->containsFields(Entity::Private));
    ASSERT_FALSE(_class->containsFields(Entity::Protected));
    _class->addExistsField(field);
    ASSERT_TRUE(_class->containsFields(Entity::Public));
    field->setSection(Entity::Private);
    ASSERT_TRUE(_class->containsFields(Entity::Private));
    field->setSection(Entity::Protected);
    ASSERT_TRUE(_class->containsFields(Entity::Protected));

    // Properties
    ASSERT_FALSE(_class->anyProperties());
    auto property = _class->addProperty("some name", Common::ID::nullID());
    ASSERT_TRUE(_class->anyProperties());
    ASSERT_EQ(property, _class->property(property->name()));
    ASSERT_TRUE(_class->containsProperty(property->name()));
    ASSERT_FALSE(_class->containsProperty("foobar"));
    _class->removeProperty(property->name());
    ASSERT_FALSE(_class->anyProperties());
    _class->addExistsProperty(property);

    test_copy_move(Class, _class)
}

TEST_F(Enteties, OptionalClassMethods)
{
    ASSERT_TRUE(_class->properties().isEmpty());
    ASSERT_TRUE(_class->optionalMethods(Entity::Public).isEmpty());

    // Add some method
    auto int_ = m_GlobalDb->typeByName("int");
    ASSERT_TRUE(!!int_);
    Entity::SharedProperty p1 = _class->addProperty("p1", int_->id());
    p1->setTypeSearcher(m_GlobalDb);
    p1->addGetter("getSmth");
    auto getter = p1->getter();
    ASSERT_FALSE(_class->optionalMethods(Entity::Public).isEmpty());
    ASSERT_EQ(getter, _class->optionalMethods(Entity::Public)[0]);

    // Check weakness of relation
    p1->deleteGetter();
    ASSERT_TRUE(!!getter);
    ASSERT_EQ(getter.use_count(), 1);
    ASSERT_TRUE(_class->optionalMethods(Entity::Public).isEmpty());

    // Check disconnect and removing property
    getter = p1->addGetter("getSmth").getter();
    auto notifier = p1->addNotifier("smthChanged").notifier();
    ASSERT_EQ(notifier, _class->optionalMethods(Entity::None)[0]);
    auto optionalMethods = _class->optionalMethods(Entity::All);
    ASSERT_EQ(optionalMethods.count(), 2);

    _class->removeProperty(p1->name());
    ASSERT_EQ(optionalMethods.count(), 2);
    ASSERT_TRUE(_class->optionalMethods(Entity::All).isEmpty());

    // Check all methods
    Entity::MethodsList tmpMethods;
    tmpMethods << _class->addNewMethod();
    tmpMethods << _class->addNewProperty()->addGetter().getter();
    Entity::MethodsList methods = _class->allMethods(Entity::All);
    ASSERT_TRUE(boost::range::equal(tmpMethods, methods));
}

TEST_F(Enteties, OptionaClassFields)
{
    ASSERT_TRUE(_class->fields().isEmpty());
    ASSERT_TRUE(_class->optionalFields(Entity::Private).isEmpty());

    // FIXME: set appropriate type
    Entity::SharedProperty p = _class->addProperty("p0", Common::ID::nullID());
    ASSERT_FALSE(_class->optionalFields(Entity::Private).isEmpty());
    ASSERT_EQ(_class->optionalFields(Entity::Private)[0], p->field());

    auto f1 = p->field();
    p->addField("foo"); // add new remove old
    ASSERT_NE(p->field(), f1);
    ASSERT_EQ(f1.use_count(), 1);
    ASSERT_EQ(_class->optionalFields(Entity::Private)[0], p->field());

    // Check a few fields
    // FIXME: set appropriate type
    auto p1 = _class->addProperty("p1", Common::ID::nullID());
    ASSERT_EQ(Entity::FieldsList({p1->field(), p->field()}).toList().toSet(),
              _class->optionalFields(Entity::Private).toList().toSet());
}

TEST_F(Enteties, Union)
{
    auto field = _union->addField("some field", Common::ID::nullID());
    ASSERT_EQ(field, _union->getField(field->name()));
    ASSERT_TRUE(_union->containsField(field->name()));
    _union->removeField(field->name());
    ASSERT_EQ(nullptr, _union->getField(field->name()));
    ASSERT_FALSE(_union->containsField(field->name()));

    _union->addExistsField(field);
    test_copy_move(Union, _union)
}

TEST_F(Enteties, Enum)
{
    ASSERT_EQ(Common::ID::nullID(), _enum->enumTypeId());
    const Common::ID newId(Common::ID::firstFreeID().value() + 1);
    _enum->setEnumTypeId(newId);
    ASSERT_EQ(newId, _enum->enumTypeId());

    auto element = _enum->addElement("new element");
    ASSERT_EQ(*element, *_enum->element(element->name()));
    ASSERT_TRUE(_enum->containsElement(element->name()));
    _enum->removeEnumerator(element);
    ASSERT_EQ(nullptr, _enum->element(element->name()));
    ASSERT_FALSE(_enum->containsElement(element->name()));

    _enum->addExistsEnumerator(element);
    test_copy_move(Enum, _enum)
}

TEST_F(Enteties, TemplateClass)
{
    ASSERT_TRUE(_templateClass->templateParameters().empty());
    auto p = _templateClass->addTemplateParameter(Common::ID::firstFreeID() + 1);
    auto p1 = _templateClass->addTemplateParameter(Common::ID::firstFreeID() + 2);
    ASSERT_EQ(_templateClass->getTemplateParameter(p.first), p);
    ASSERT_FALSE(_templateClass->templateParameters().empty());
    ASSERT_TRUE(_templateClass->contains(p.first));
    ASSERT_TRUE(_templateClass->contains(p1.first));
    _templateClass->removeParameter(p.first);
    ASSERT_FALSE(_templateClass->contains(p.first));

    ASSERT_TRUE(!!_templateClass->database());
    ASSERT_EQ(_templateClass->database()->scopes().count(), 1);

    ASSERT_TRUE(_templateClass->localTypes().empty());
    auto localType = _templateClass->addLocalType("T");
    auto localType1 = _templateClass->addLocalType("T1");
    ASSERT_TRUE(!!localType);
    ASSERT_TRUE(!!localType1);
    ASSERT_FALSE(_templateClass->localTypes().empty());
    ASSERT_TRUE(_templateClass->containsLocalType(localType1->id()));
    _templateClass->removeLocalType(localType1->id());
    ASSERT_FALSE(_templateClass->containsLocalType(localType1->id()));

    test_copy_move(TemplateClass, _templateClass)
}
