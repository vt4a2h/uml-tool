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

#include "TestProjectTranslator.h"

#include <templates.cpp>

TEST_F(ProjectTranslatorTest, Type)
{
    QString futureResult("int");
    auto code(_translator->translate(_int));
    ASSERT_EQ(futureResult, code.toHeader);

    futureResult = "project_scope::foo_scope::Foo";
    auto scopeFoo = _projectScope->addChildScope("foo_scope");
    auto foo = scopeFoo->addType("Foo");
    code = _translator->translate(foo);
    ASSERT_EQ(futureResult, code.toHeader);
}

TEST_F(ProjectTranslatorTest, ExtendedType)
{
    entity::SharedExtendedType type = _projectScope->addType<entity::ExtendedType>();
    type->setTypeId(_int->id());

    QString futureResult("int");
    auto code(_translator->translate(type));
    ASSERT_EQ(futureResult.toStdString(), code.toHeader.toStdString());

    futureResult = "const int";
    type->setConstStatus(true);
    code = _translator->translate(type);
    ASSERT_EQ(futureResult, code.toHeader);

    futureResult = "const int &";
    type->addLinkStatus();
    code = _translator->translate(type);
    ASSERT_EQ(futureResult, code.toHeader);

    futureResult = "const int &&";
    type->addLinkStatus();
    code = _translator->translate(type);
    ASSERT_EQ(futureResult, code.toHeader);

    type->removeLinkStatus();
    type->removeLinkStatus();

    futureResult = "const int * const";
    type->addPointerStatus(true);
    code = _translator->translate(type);
    ASSERT_EQ(futureResult, code.toHeader);

    type->removePointerStatus();
    type->setConstStatus(false);

    futureResult = "vector<int>";
    auto vector = _globalScope->addType("vector");
    type->setTypeId(vector->id());
    type->addTemplateParameter(_int->id());
    code = _translator->translate(type);
    ASSERT_EQ(futureResult, code.toHeader);

    futureResult = "using Ints = vector<int>;";
    type->setName("Ints");
    code = _translator->translate(type, translation::ProjectTranslator::WithAlias |
                                        translation::ProjectTranslator::WithNamespace);
    ASSERT_EQ(futureResult, code.toHeader);

    futureResult = "std::set<int>";
    _projectScope->removeType(type->id());
    type = _projectScope->addType<entity::ExtendedType>();
    auto stdScope = _globalScope->addChildScope("std");
    auto set = stdScope->addType("set");
    type->setTypeId(set->id());
    type->addTemplateParameter(_int->id());
    code = _translator->translate(type);
    ASSERT_EQ(futureResult.toStdString(), code.toHeader.toStdString());
}

TEST_F(ProjectTranslatorTest, Field)
{
    auto field = std::make_shared<entity::Field>("Number", _int->id());
    field->setPrefix("m_");

    QString futureResult("int m_Number");
    auto code(_translator->translate(field));
    ASSERT_EQ(futureResult.toStdString(), code.toHeader.toStdString());

    futureResult = "static int number_";
    field->setName("number");
    field->removePrefix();
    field->setSuffix("_");
    field->addKeyword(entity::FieldStatic);
    code = _translator->translate(field);
    ASSERT_EQ(futureResult, code.toHeader);

    futureResult = "static const int number_";
    auto type = _globalScope->addType<entity::ExtendedType>();
    type->setTypeId(_int->id());
    type->setConstStatus(true);
    field->setTypeId(type->id());
    code = _translator->translate(field);
    ASSERT_EQ(futureResult.toStdString(), code.toHeader.toStdString());
}

TEST_F(ProjectTranslatorTest, ClassMethod)
{
    auto method = std::make_shared<entity::ClassMethod>("calc");
    method->setConstStatus(true);

    QString futureResult("void calc() const");
    auto voidType = _globalScope->addType("void");
    method->setReturnTypeId(voidType->id());
    auto code(_translator->translate(method));
    ASSERT_EQ(futureResult.toStdString(), code.toHeader.toStdString());

    futureResult = "double sum(double a, double b) const";
    auto doubleType = _globalScope->addType("double");
    method->setName("sum");
    method->setReturnTypeId(doubleType->id());
    method->addParameter("a", doubleType->id());
    method->addParameter("b", doubleType->id());
    code = _translator->translate(method);
    ASSERT_EQ(futureResult.toStdString(), code.toHeader.toStdString());

    futureResult = "ps::Foo *getFoo(const QString &id) const";
    auto ps = _projectDb->addScope("ps");

    auto foo = ps->addType("Foo");
    auto fooExt = ps->addType<entity::ExtendedType>();
    fooExt->addPointerStatus();
    fooExt->setTypeId(foo->id());

    auto qstr = _globalScope->addType("QString");
    auto qstrExt = _globalScope->addType<entity::ExtendedType>();
    qstrExt->addLinkStatus();
    qstrExt->setTypeId(qstr->id());
    qstrExt->setConstStatus(true);

    method->setName("getFoo");
    method->setReturnTypeId(fooExt->id());
    method->removeParameter("a");
    method->removeParameter("b");
    method->addParameter("id", qstrExt->id());

    code = _translator->translate(method, translation::ProjectTranslator::WithNamespace);
    ASSERT_EQ(futureResult.toStdString(), code.toHeader.toStdString());

    futureResult = "explicit Foo(const QString &name)";
    method = std::make_shared<entity::ClassMethod>("Foo");
    method->addLhsIdentificator(entity::LhsIdentificator::Explicit);
    method->addParameter("name", qstrExt->id());
    code = _translator->translate(method);
    ASSERT_EQ(futureResult.toStdString(), code.toHeader.toStdString());

    futureResult = "virtual ps::Foo *make() = 0";
    method = std::make_shared<entity::ClassMethod>("make");
    method->setReturnTypeId(fooExt->id());
    method->setRhsIdentificator(entity::RhsIdentificator::PureVirtual);
    method->addLhsIdentificator(entity::LhsIdentificator::Virtual);
    code = _translator->translate(method, translation::ProjectTranslator::WithNamespace);
    ASSERT_EQ(futureResult, code.toHeader);

    futureResult = "ps::Foo *make() override";
    method->removeLhsIdentificator(entity::LhsIdentificator::Virtual);
    method->setRhsIdentificator(entity::RhsIdentificator::Override);
    code = _translator->translate(method, translation::ProjectTranslator::WithNamespace);
    ASSERT_EQ(futureResult, code.toHeader);
}

TEST_F(ProjectTranslatorTest, TemplateClassMethod)
{
    entity::SharedTemplateClassMethod method(std::make_shared<entity::TemplateClassMethod>("swap"));

    QString futureResult("template <>\nswap()");
    auto code(_translator->translate(method));
    ASSERT_EQ(futureResult, code.toHeader);

    futureResult = "template <class T>\nswap()";
    auto t = method->addLocalType("T");
    method->addTemplateParameter(t->id());
    code = _translator->translate(method);
    ASSERT_EQ(futureResult, code.toHeader);

    futureResult = "template <class T>\nswap(T *first, T *second)";
    auto ptrT = method->addLocalType<entity::ExtendedType>();
    ptrT->setTypeId(t->id());
    ptrT->addPointerStatus();
    method->addParameter("first", ptrT->id());
    method->addParameter("second", ptrT->id());
    code = _translator->translate(method);
    ASSERT_EQ(futureResult, code.toHeader);

    futureResult = "template <class T = project_scope::Foo>\n"
                   "std::shared_ptr<T> make(const QString &name)";
    method = std::make_shared<entity::TemplateClassMethod>("make");
    auto qstring = _globalScope->addType("QString");
    auto constLinkToQstr = _globalScope->addType<entity::ExtendedType>();
    constLinkToQstr->setTypeId(qstring->id());
    constLinkToQstr->addLinkStatus();
    constLinkToQstr->setConstStatus(true);
    method->addParameter("name", constLinkToQstr->id());

    auto foo = _projectScope->addType("Foo");
    t = method->addLocalType("T");
    method->addTemplateParameter(t->id(), foo->id());

    auto stdScope = _globalDb->addScope("std");
    auto sharedPointer = stdScope->addType("shared_ptr");
    auto sharedPointerToT = stdScope->addType<entity::ExtendedType>();
    sharedPointerToT->setTypeId(sharedPointer->id());
    sharedPointerToT->addTemplateParameter(t->id());
    method->setReturnTypeId(sharedPointerToT->id());

    code = _translator->translate(method, translation::ProjectTranslator::WithNamespace);
    ASSERT_EQ(futureResult, code.toHeader);
}

TEST_F(ProjectTranslatorTest, Enum)
{
    auto fooEnum = _projectScope->addType<entity::Enum>("Foo");

    QString futureResult("enum Foo {};");
    auto code(_translator->translate(fooEnum));
    ASSERT_EQ(futureResult, code.toHeader);

    futureResult = "enum class Foo {};";
    fooEnum->setStrongStatus(true);
    code = _translator->translate(fooEnum);
    ASSERT_EQ(futureResult, code.toHeader);

    futureResult = "enum class Foo : int {};";
    fooEnum->setEnumTypeId(_int->id());
    code = _translator->translate(fooEnum);
    ASSERT_EQ(futureResult, code.toHeader);

    futureResult = "enum class Foo : int {bar, baz};";
    fooEnum->addElement("bar");
    fooEnum->addElement("baz");
    code = _translator->translate(fooEnum);
    ASSERT_EQ(futureResult, code.toHeader);

    futureResult = "enum class Foo : int {bar = 0, baz = 1};";
    code = _translator->translate(fooEnum, translation::ProjectTranslator::GenerateNumbers);
    ASSERT_EQ(futureResult, code.toHeader);
}

TEST_F(ProjectTranslatorTest, Union)
{
    auto fooUnion = _projectScope->addType<entity::Union>("Foo");

    QString futureResult("union Foo {};");
    auto code(_translator->translate(fooUnion));
    ASSERT_EQ(futureResult, code.toHeader);

    futureResult = "union Foo {\n    double a;\n    int b;\n};";
    auto typeDouble = _globalScope->addType("double");
    fooUnion->addField("a", typeDouble->id());
    fooUnion->addField("b", _int->id());
    code = _translator->translate(fooUnion);
    ASSERT_EQ(futureResult, code.toHeader);
}

TEST_F(ProjectTranslatorTest, Class)
{
    auto fooClass = _projectScope->addType<entity::Class>("Foo");

    QString futureResult("class Foo {};");
    auto code(_translator->translate(fooClass));
    ASSERT_EQ(futureResult.toStdString(), code.toHeader.toStdString());

    futureResult = "struct Foo {};";
    fooClass->setKind(entity::StructType);
    code = _translator->translate(fooClass);
    ASSERT_EQ(futureResult.toStdString(), code.toHeader.toStdString());

    futureResult = "class Foo : public Bar {};";
    fooClass->setKind(entity::ClassType);
    auto barClass = _projectScope->addType<entity::Class>("Bar");
    fooClass->addParent(barClass->id(), entity::Public);
    code = _translator->translate(fooClass);
    ASSERT_EQ(futureResult.toStdString(), code.toHeader.toStdString());

    futureResult = "class Foo : public Bar, protected Baz {};";
    auto bazClass = _projectScope->addType<entity::Class>("Baz");
    fooClass->addParent(bazClass->id(), entity::Protected);
    code = _translator->translate(fooClass);
    ASSERT_EQ(futureResult.toStdString(), code.toHeader.toStdString());

    fooClass->removeParent(barClass->id());
    fooClass->removeParent(bazClass->id());

    futureResult = QString("class Foo\n{\n"
                           "%1private:\n"
                           "%1%1int a_;\n"
                           "%1%1Baz b_;\n"
                           "};").arg(INDENT);
    auto aField = fooClass->addField("a", _int->id());
    aField->setSection(entity::Private);
    aField->setSuffix("_");
    auto bField = fooClass->addField("b", bazClass->id());
    bField->setSection(entity::Private);
    bField->setSuffix("_");
    code = _translator->translate(fooClass);
    ASSERT_EQ(futureResult.toStdString(), code.toHeader.toStdString());

    fooClass->removeField("a");
    fooClass->removeField("b");

    futureResult = QString("class Foo\n{\n"
                           "%1public:\n"
                           "%1%1int a;\n\n"
                           "%1protected:\n"
                           "%1%1int b;\n\n"
                           "%1private:\n"
                           "%1%1int c;\n"
                           "};").arg(INDENT);
    aField = fooClass->addField("a", _int->id());
    bField = fooClass->addField("b", _int->id());
    bField->setSection(entity::Protected);
    auto cField = fooClass->addField("c", _int->id());
    cField->setSection(entity::Private);
    code = _translator->translate(fooClass);
    ASSERT_EQ(futureResult, code.toHeader);

    fooClass->removeField("a");
    fooClass->removeField("b");


    futureResult = QString("class Foo\n{\n"
                           "%1public:\n"
                           "%1%1int c() const;\n"
                           "%1%1void setC(const int &newC);\n\n"
                           "%1private:\n"
                           "%1%1int c;\n"
                           "};").arg(INDENT);
    auto cGetter = fooClass->makeMethod("c");
    cGetter->setConstStatus(true);
    cGetter->setReturnTypeId(cField->typeId());
    cGetter->setSection(entity::Public);

    auto cSetter = fooClass->makeMethod("setC");
    auto voidType = _globalScope->addType("void");
    cSetter->setReturnTypeId(voidType->id());
    cSetter->setSection(entity::Public);
    auto constLintToInt = _projectScope->addType<entity::ExtendedType>();
    constLintToInt->setTypeId(cField->typeId());
    constLintToInt->setConstStatus(true);
    constLintToInt->addLinkStatus();
    cSetter->addParameter("newC", constLintToInt->id());

    code = _translator->translate(fooClass);
    ASSERT_EQ(futureResult, code.toHeader);
}

TEST_F(ProjectTranslatorTest, ClassWithProperties)
{
    auto cl = _projectScope->addType<entity::Class>("Baz");
    auto prop = cl->addProperty("a", _int->id());
    prop->addGetter("getA").addSetter("setA").addNotifier("aChanged");

    QString expect = QString("class Baz{\n"
                             "Q_OBJECT"
                             "\n\n"
                             "int a MEMBER m_a READ getA WRITE setA NOTIFY aChanged"
                             "\n\n"
                             "%1public:\n"
                             "%1%1int getA() const;\n"
                             "%1public SLOTS:\n"
                             "%1%1void setA(int a);\n\n"
                             "%1signals:\n"
                             "%1%1void aChanged();\n"
                             "};").arg(INDENT);
    ASSERT_EQ(expect.toStdString(), _translator->translate(cl).toHeader.toStdString());

    // Add all fields
    prop->addResetter("resetA")
         .setRevision(1)
         .addDesignableGetter("isDisignable")
         .addScriptableGetter("isScriptable")
         .setStored(false)
         .setUser(true)
         .setConstant(true)
         .setFinal(true);

    expect = QString("class Baz{\n"
                     "Q_OBJECT\n\n"
                     "int a MEMBER m_a READ getA WRITE setA RESET resetA NOTIFY aChanged REVISION 1 "
                            "DESIGNABLE isDisignable SCRIPTABLE isScriptable STORED false "
                            "USER true CONSTANT FINAL\n\n"
                     "%1public:\n"
                     "%1%1int getA() const;\n"
                     "%1%1bool isDisignable();\n"
                     "%1%1bool isScriptable();\n"
                     "%1public SLOTS:\n"
                     "%1%1void setA(int a);\n"
                     "%1%1void resetA();\n\n"
                     "%1signals:\n"
                     "%1%1void aChanged();\n"
                     "};").arg(INDENT);

    ASSERT_EQ(expect.toStdString(), _translator->translate(cl).toHeader.toStdString());

    // TODO: add fields generation
}

TEST_F(ProjectTranslatorTest, TemplateClass)
{
    QString futureResult = QString("template <class T>\n"
                                   "struct Node"
                                   "{\n"
                                   "%1T data_;\n"
                                   "%1Node *next;\n"
                                   "};").arg(INDENT);
    auto nodeStruct = _projectScope->addType<entity::TemplateClass>("Node");
    nodeStruct->setKind(entity::StructType);

    auto tType = nodeStruct->addLocalType("T");
    nodeStruct->addTemplateParameter(tType->id());

    auto dataField = nodeStruct->addField("data", tType->id());
    dataField->setSuffix("_");

    auto ptrNode = _projectScope->addType<entity::ExtendedType>();
    ptrNode->setTypeId(nodeStruct->id());
    ptrNode->addPointerStatus();
    nodeStruct->addField("next", ptrNode->id());

    auto code(_translator->translate(nodeStruct));
    ASSERT_EQ(futureResult, code.toHeader);
}

TEST_F(ProjectTranslatorTest, ClassImplementation)
{
    QString futureResult("int Foo::getC() const\n"
                         "{\n"
                         "}\n");

    entity::SharedClass classFoo(_projectScope->addType<entity::Class>("Foo"));
    entity::SharedMethod getterForC(classFoo->makeMethod("getC"));
    getterForC->setConstStatus(true);
    getterForC->setReturnTypeId(_int->id());

    translation::Code code(_translator->generateClassMethodsImpl(classFoo));
    ASSERT_TRUE(code.toHeader.isEmpty())
            << "Header should be empty!";
    ASSERT_EQ(futureResult.toStdString(), code.toSource.toStdString());

    futureResult = "int Foo::getC() const\n"
                   "{\n"
                   "}\n"
                   "\n"
                   "void Foo::setC(int c)\n"
                   "{\n"
                   "}\n";
    entity::SharedMethod setterForC(classFoo->makeMethod("setC"));
    entity::SharedType voidType(_globalScope->addType("void"));
    setterForC->setReturnTypeId(voidType->id());
    setterForC->addParameter("c", _int->id());

    code = _translator->generateClassMethodsImpl(classFoo);
    ASSERT_TRUE(code.toHeader.isEmpty()) << "Header should be empty!";
    ASSERT_EQ(futureResult, code.toSource);

    QString futureResultH("template <class T>\n"
                          "void Foo::swap(T &src)\n"
                          "{\n"
                          "}\n");
    entity::SharedTemplateClassMethod swapMethod(classFoo->makeMethod<entity::TemplateClassMethod>("swap"));
    swapMethod->setReturnTypeId(voidType->id());
    entity::SharedType typeT(swapMethod->addLocalType("T"));
    swapMethod->addTemplateParameter(typeT->id());
    entity::SharedExtendedType typeTLink(swapMethod->addLocalType<entity::ExtendedType>());
    typeTLink->setTypeId(typeT->id());
    typeTLink->addLinkStatus();
    swapMethod->addParameter("src", typeTLink->id());

    code = _translator->generateClassMethodsImpl(classFoo);
    ASSERT_FALSE(code.toHeader.isEmpty()) << "Header should be generated!";
    ASSERT_FALSE(code.toSource.isEmpty()) << "Source file should be generated!";
    ASSERT_EQ(futureResultH, code.toHeader);
    ASSERT_EQ(futureResult, code.toSource);
}

TEST_F(ProjectTranslatorTest, TemplateClassImplementation)
{
    QString futureResult("\ntemplate <class Value, class Deleter>\n"
                         "void ScopedPointer<Value, Deleter>::reset(Value *other)\n"
                         "{\n"
                         "}");

    entity::SharedType voidType(_globalScope->addType("void"));

    entity::SharedScope _std(_globalDb->addScope("std"));
    entity::SharedType dd(_std->addType("default_delete"));

    entity::SharedTemplateClass scopedPointer(_projectScope->addType<entity::TemplateClass>("ScopedPointer"));
    entity::SharedType value(scopedPointer->addLocalType("Value"));
    entity::SharedType deleter(scopedPointer->addLocalType("Deleter"));
    entity::SharedExtendedType defaultDelete(scopedPointer->addLocalType<entity::ExtendedType>());
    defaultDelete->setTypeId(dd->id());
    defaultDelete->addTemplateParameter(value->id());
    scopedPointer->addTemplateParameter(value->id());
    scopedPointer->addTemplateParameter(deleter->id(), defaultDelete->id());

    entity::SharedMethod resetMethod(scopedPointer->makeMethod("reset"));
    resetMethod->setReturnTypeId(voidType->id());
    entity::SharedExtendedType valuePtr(scopedPointer->addLocalType<entity::ExtendedType>());
    valuePtr->setTypeId(value->id());
    valuePtr->addPointerStatus();
    resetMethod->addParameter("other", valuePtr->id());

    translation::Code code(_translator->generateClassMethodsImpl(scopedPointer));
    ASSERT_EQ(futureResult, code.toHeader);
}
