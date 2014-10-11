#include <gtest/gtest.h>
#include "helpers.h"
#include "TestDepthSearch.h"
#include "TestTypeMaker.h"
#include "TestRelationMaker.h"
#include "TestProjectTranslator.h"
#include "TestFileMaker.h"
#include "TestJson.h"
#include "TestProjectMaker.h"
#include <helpfunctions.h>
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
    ASSERT_EQ(futureResult, code.toHeader);

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
    code = _translator->translate(type, translator::ProjectTranslator::WithAlias |
                                        translator::ProjectTranslator::WithNamespace);
    ASSERT_EQ(futureResult, code.toHeader);

    futureResult = "std::set<int>";
    _projectScope->removeType(type->id());
    type = _projectScope->addType<entity::ExtendedType>();
    auto stdScope = _globalScope->addChildScope("std");
    auto set = stdScope->addType("set");
    type->setTypeId(set->id());
    type->addTemplateParameter(_int->id());
    code = _translator->translate(type);
    ASSERT_EQ(futureResult, code.toHeader);
}

TEST_F(ProjectTranslatorTest, Field)
{
    auto field = std::make_shared<entity::Field>("Number", _int->id());
    field->setPrefix("m_");

    QString futureResult("int m_Number");
    auto code(_translator->translate(field));
    ASSERT_EQ(futureResult, code.toHeader);

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
    ASSERT_EQ(futureResult, code.toHeader);
}

TEST_F(ProjectTranslatorTest, ClassMethod)
{
    auto method = std::make_shared<entity::ClassMethod>("calc");
    method->setConstStatus(true);

    QString futureResult("void calc() const");
    auto voidType = _globalScope->addType("void");
    method->setReturnTypeId(voidType->id());
    auto code(_translator->translate(method));
    ASSERT_EQ(futureResult, code.toHeader);

    futureResult = "double sum(double a, double b) const";
    auto doubleType = _globalScope->addType("double");
    method->setName("sum");
    method->setReturnTypeId(doubleType->id());
    method->addParameter("a", doubleType->id());
    method->addParameter("b", doubleType->id());
    code = _translator->translate(method);
    ASSERT_EQ(futureResult, code.toHeader);

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

    code = _translator->translate(method);
    ASSERT_EQ(futureResult, code.toHeader);

    futureResult = "explicit Foo(const QString &name)";
    method = std::make_shared<entity::ClassMethod>("Foo");
    method->addLhsIdentificator(entity::Explicit);
    method->addParameter("name", qstrExt->id());
    code = _translator->translate(method);
    ASSERT_EQ(futureResult, code.toHeader);

    futureResult = "virtual ps::Foo *make() = 0";
    method = std::make_shared<entity::ClassMethod>("make");
    method->setReturnTypeId(fooExt->id());
    method->setRhsIdentificator(entity::PureVirtual);
    method->addLhsIdentificator(entity::Virtual);
    code = _translator->translate(method);
    ASSERT_EQ(futureResult, code.toHeader);

    futureResult = "ps::Foo *make() override";
    method->removeLhsIdentificator(entity::Virtual);
    method->setRhsIdentificator(entity::Override);
    code = _translator->translate(method);
    ASSERT_EQ(futureResult, code.toHeader);
}

TEST_F(ProjectTranslatorTest, TemplateClassMethod)
{
    entity::SharedTemplateClassMethod method(std::make_shared<entity::TemplateClassMethod>("swap"));

    QString futureResult("template <>\nswap()");
    auto code(_translator->translate(method));
    ASSERT_EQ(futureResult, code.toHeader);

    futureResult = "template <class T>\nswap()";
    auto t = method->addLocaleType("T");
    method->addTemplateParameter(t->id());
    code = _translator->translate(method);
    ASSERT_EQ(futureResult, code.toHeader);

    futureResult = "template <class T>\nswap(T *first, T *second)";
    auto ptrT = method->addLocaleType<entity::ExtendedType>();
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
    t = method->addLocaleType("T");
    method->addTemplateParameter(t->id(), foo->id());

    auto stdScope = _globalDb->addScope("std");
    auto sharedPointer = stdScope->addType("shared_ptr");
    auto sharedPointerToT = stdScope->addType<entity::ExtendedType>();
    sharedPointerToT->setTypeId(sharedPointer->id());
    sharedPointerToT->addTemplateParameter(t->id());
    method->setReturnTypeId(sharedPointerToT->id());

    code = _translator->translate(method);
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
    fooEnum->addVariable("bar");
    fooEnum->addVariable("baz");
    code = _translator->translate(fooEnum);
    ASSERT_EQ(futureResult, code.toHeader);

    futureResult = "enum class Foo : int {bar = 0, baz = 1};";
    code = _translator->translate(fooEnum, translator::ProjectTranslator::GenerateNumbers);
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
    ASSERT_EQ(futureResult, code.toHeader);

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

    auto tType = nodeStruct->addLocaleType("T");
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

    translator::Code code(_translator->generateClassMethodsImpl(classFoo));
    ASSERT_TRUE(code.toHeader.isEmpty())
            << "Header should be empty!";
    ASSERT_EQ(futureResult, code.toSource);

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
    entity::SharedType typeT(swapMethod->addLocaleType("T"));
    swapMethod->addTemplateParameter(typeT->id());
    entity::SharedExtendedType typeTLink(swapMethod->addLocaleType<entity::ExtendedType>());
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
    QString futureResult("template <class Value, class Deleter>\n"
                         "void ScopedPointer<Value, Deleter>::reset(Value *other)\n"
                         "{\n"
                         "}\n");

    entity::SharedType voidType(_globalScope->addType("void"));

    entity::SharedScope _std(_globalDb->addScope("std"));
    entity::SharedType dd(_std->addType("default_delete"));

    entity::SharedTemplateClass scopedPointer(_projectScope->addType<entity::TemplateClass>("ScopedPointer"));
    entity::SharedType value(scopedPointer->addLocaleType("Value"));
    entity::SharedType deleter(scopedPointer->addLocaleType("Deleter"));
    entity::SharedExtendedType defaultDelete(scopedPointer->addLocaleType<entity::ExtendedType>());
    defaultDelete->setTypeId(dd->id());
    defaultDelete->addTemplateParameter(value->id());
    scopedPointer->addTemplateParameter(value->id());
    scopedPointer->addTemplateParameter(deleter->id(), defaultDelete->id());

    entity::SharedMethod resetMethod(scopedPointer->makeMethod("reset"));
    resetMethod->setReturnTypeId(voidType->id());
    entity::SharedExtendedType valuePtr(scopedPointer->addLocaleType<entity::ExtendedType>());
    valuePtr->setTypeId(value->id());
    valuePtr->addPointerStatus();
    resetMethod->addParameter("other", valuePtr->id());

    translator::Code code(_translator->generateClassMethodsImpl(scopedPointer));
    ASSERT_EQ(futureResult, code.toHeader);
}

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

    multAssociation->setFieldtypeId(valueType->id());
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

    association->setFieldtypeId(type->id());
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
    depMethod->setReturnTypeId(VOID_ID);
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

TEST_F(TypeMaker, MakesRightTypes)
{
    for (auto typeName : _typesNames)
        EXPECT_EQ(_types[typeName]->type(),
                  static_cast<entity::UserType>(_typesNames.indexOf(typeName))
                  ) << typeName.toStdString() << " class object sholud has valid type id";
}

TEST_F(DepthSearch, ScopeSearchWorks)
{
    entity::SharedScope p(nullptr);

    search_circle(_scopes, depthScopeSearch, scope)
    invalid_case(depthScopeSearch, "foobarid")
}

TEST_F(DepthSearch, TypeSearchWorks)
{
    entity::SharedType p(nullptr);

    search_circle(_types, depthTypeSearch, type)
    invalid_case(depthTypeSearch, "foobarbaz")
}

TEST_F(FileMaker, MakeFileStructure)
{
    m_Directory.setPath(m_RootPath + m_Sep + "dir_name");
    m_Directory.write();
    QFileInfo info(m_Directory.path());

    EXPECT_TRUE(m_Directory.errorList()->isEmpty())
            << "Directory should be created without any errors";
    EXPECT_TRUE(info.isDir())
            << "VirtualDirectory::write should create directory.";
    EXPECT_TRUE(info.isWritable())
            << "Directory should be writable.";

    generator::SharedVirtualDirectory dir1(m_Directory.addDirectory("dir1"));

    EXPECT_TRUE(m_Directory.constainsDirectory("dir1"))
            << "Directory should contains nested directory.";
    EXPECT_FALSE(m_Directory.constainsDirectory("dir123"))
            << "Directory should not contains wrong nested directory.";

    m_Directory.write();
    info = dir1->path();

    EXPECT_TRUE(m_Directory.errorList()->isEmpty())
            << "Nested directory should be created without any errors";
    EXPECT_TRUE(info.isDir())
            << "VirtualDirectory::write should create nested directory.";
    EXPECT_TRUE(info.isWritable())
            << "Nested directory should be writable.";

    generator::SharedVirtualFile file1(m_Directory.addFile("foo.cpp"));

    EXPECT_TRUE(m_Directory.constainsFile("foo.cpp"))
            << "Directory should contains nested file.";
    EXPECT_FALSE(m_Directory.constainsFile("dir1"))
            << "Directory should not contains wrong nested file.";

    QString fileData("int main()\n{\n}");
    file1->setData(fileData);
    m_Directory.write();
    info = file1->path();

    EXPECT_TRUE(m_Directory.errorList()->isEmpty())
            << "File should be created without any errors";
    EXPECT_TRUE(info.isFile())
            << "VirtualFile::write should create a file.";
    EXPECT_TRUE(info.isReadable())
            << "File should be readable.";

    QFile f(file1->path());
    EXPECT_TRUE(f.open(QIODevice::ReadOnly | QIODevice::Text))
            << "File should be openable";
    QTextStream s(&f);
    EXPECT_EQ(fileData, s.readAll())
            << "File should contains right data";
    f.close();
}

TEST_F(FileJson, TypeJson)
{
    entity::SharedType type(std::make_shared<entity::Type>("stub_name", "stub_scope_id"));
    type->writeToFile(m_JsonFileName);

    auto type_comp(std::make_shared<entity::Type>());
    json_eq(type, type_comp, "Type")
}

TEST_F(FileJson, ExtendedTypeJson)
{
    entity::SharedExtendedType type(std::make_shared<entity::ExtendedType>("stub_name", "stub_scope_id"));
    type->addLinkStatus();
    type->addPointerStatus(true);
    type->addTemplateParameter("id1");
    type->addTemplateParameter("id2");
    type->setConstStatus(true);
    type->setId("id3");
    type->writeToFile(m_JsonFileName);

    auto type_comp(std::make_shared<entity::ExtendedType>());
    json_eq(type, type_comp, "ExtendedType")
}

TEST_F(FileJson, UnionJson)
{
    entity::SharedUnion union_(std::make_shared<entity::Union>("stub_name", "stub_scope_id"));
    union_->addField("stub_name", "stub_id");
    union_->writeToFile(m_JsonFileName);

    auto union_comp(std::make_shared<entity::Union>());
    json_eq(union_, union_comp, "Union")
}

TEST_F(FileJson, EnumJson)
{
    entity::SharedEnum enum_(std::make_shared<entity::Enum>("stub_name", "stub_scope_id"));
    enum_->setStrongStatus(true);
    enum_->addVariable("a");
    enum_->addVariable("b");
    enum_->writeToFile(m_JsonFileName);

    auto enum_comp(std::make_shared<entity::Enum>());
    json_eq(enum_, enum_comp, "Enum")
}

TEST_F(FileJson, ClassJson)
{
    entity::SharedClass class_(std::make_shared<entity::Class>("stub_name", "stub_scope_id"));
    class_->addParent("stub_parent_id", entity::Protected);
    class_->addField("stub_field_name", "stub_type_id");
    class_->addField("stub_field_name_1", "stub_type_id_1");
    class_->makeMethod("stub_method_name");
    class_->makeMethod("stub_method_name_1");
    class_->writeToFile(m_JsonFileName);

    auto class_comp(std::make_shared<entity::Class>());
    json_eq(class_, class_comp, "Class");
}

TEST_F(FileJson, FieldJson)
{
    entity::SharedField field(std::make_shared<entity::Field>("stub_name", "stub_id"));
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
    method->setReturnTypeId("stub_return_type_id");
    method->setRhsIdentificator(entity::Final);
    method->setSection(entity::Private);
    method->addLhsIdentificator(entity::Inline);
    method->addParameter("stub_parameter", "stub_id");
    method->addParameter("stub_parameter_1", "stub_id_1");
    method->writeToFile(m_JsonFileName);

    auto method_comp(std::make_shared<entity::ClassMethod>());
    json_eq(method, method_comp, "ClassMethod")
}

TEST_F(FileJson, TemplateClassJson)
{
    entity::SharedTemplateClass class_(std::make_shared<entity::TemplateClass>("stub_name", "stub_scope_id"));
    class_->addTemplateParameter("stub_type_id", "stub_default_id");
    class_->addTemplateParameter("stub_type_id_1", "stub_default_id_1");
    EXPECT_TRUE(class_->addLocaleType("type").operator bool())
            << "Locale type must be added.";
    EXPECT_TRUE(class_->addLocaleType("type_1").operator bool())
            << "Locale type must be added.";
    class_->writeToFile(m_JsonFileName);

    auto class_comp(std::make_shared<entity::TemplateClass>());
    json_eq(class_, class_comp, "TemplateClass")
}

TEST_F(FileJson, TemplateClassMethodJson)
{
    entity::SharedTemplateClassMethod method(std::make_shared<entity::TemplateClassMethod>("stub name"));
    EXPECT_TRUE(method->addLocaleType("type").operator bool())
            << "Locale type must be added.";
    EXPECT_TRUE(method->addLocaleType("type_1").operator bool())
            << "Locale type must be added.";
    method->writeToFile(m_JsonFileName);

    auto method_comp(std::make_shared<entity::TemplateClassMethod>());
    json_eq(method, method_comp, "TemplateClassMethod");
}

TEST_F(FileJson, ScopeJson)
{
    entity::SharedScope scope(std::make_shared<entity::Scope>("stub_scope_name", "stub_scope_id"));
    scope->addType("type");
    scope->addType("type_1");
    scope->addChildScope("scope");
    scope->addChildScope("scope_1");
    scope->writeToFile(m_JsonFileName);

    auto scope_comp(std::make_shared<entity::Scope>());
    json_eq(scope, scope_comp, "Scope");
}

TEST_F(FileJson, NodeJson)
{
    relationship::SharedNode node(std::make_shared<relationship::Node>("stub_id", relationship::ZeroOrInf));
    node->setDescription("stub_description");
    node->writeToFile(m_JsonFileName);

    auto node_comp(std::make_shared<relationship::Node>());
    json_eq(node, node_comp, "Node");
}

TEST_F(FileJson, BasicRelationJson)
{
    test_relation(Relation, [&](){ relation->setDescription("description"); })
}

TEST_F(FileJson, AssociationRelationJson)
{
    test_relation(Association, [&](){
        relation->setGetSetTypeId("get_set_type_id");
        relation->setFieldtypeId("field_type_id");
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
        auto key(m_Parameters.globalScope_->addType("key_stub"));
        auto container(m_Parameters.projectScope_->addType("container_stub"));

        relation->setContainerTypeId(container->id());
        relation->setKeyTypeId(key->id());
    })
}

TEST_F(FileJson, RealizationRelation)
{
    test_relation(Realization, [&](){
        entity::MethodsList methods;
        methods << m_Parameters.firstClass_->makeMethod("first_method")
                << m_Parameters.firstClass_->makeMethod("second_method");

        relation->addMethods(methods);
    })
}

TEST_F(ProjectMaker, MakeClass)
{
    auto scopeWork = projectDb_->addScope("work");
    auto empClass  = scopeWork->addType<entity::Class>("Employee");

    empClass->addField("firstName", string_->id(), "m_", entity::Private);
    empClass->addField("lastName",  string_->id(), "m_", entity::Private);
    empClass->addField("status",    bool_->id(),   "m_", entity::Private);
    empClass->addField("salary",    double_->id(), "m_", entity::Private);

    auto constLinkToString = globalScope_->addType<entity::ExtendedType>();
    constLinkToString->setTypeId(string_->id());
    constLinkToString->setConstStatus(true);
    constLinkToString->addLinkStatus();

    empClass->makeMethod("Employee")->setReturnTypeId(STUB_ID);

    auto parCtor = empClass->makeMethod("Employee");
    parCtor->setReturnTypeId(STUB_ID);
    parCtor->addParameter("firstName", constLinkToString->id());
    parCtor->addParameter("lastName", constLinkToString->id());

    auto firstNameGetter = empClass->makeMethod("firstName");
    firstNameGetter->setReturnTypeId(string_->id());
    firstNameGetter->setConstStatus(true);
    empClass->makeMethod("setFirstName")->addParameter("firstName", constLinkToString->id());

    auto lastNameGetter = empClass->makeMethod("lastName");
    lastNameGetter->setReturnTypeId(string_->id());
    lastNameGetter->setConstStatus(true);
    empClass->makeMethod("setLastName")->addParameter("lastName", constLinkToString->id());

    auto isHired = empClass->makeMethod("isHired");
    isHired->setReturnTypeId(bool_->id());
    isHired->setConstStatus(true);
    empClass->makeMethod("fire")->setReturnTypeId(bool_->id());
    empClass->makeMethod("hire")->setReturnTypeId(bool_->id());

    auto salaryGetter = empClass->makeMethod("salary");
    salaryGetter->setReturnTypeId(double_->id());
    salaryGetter->setConstStatus(true);
    empClass->makeMethod("setSalary")->addParameter("salary", double_->id());

    generator_->generate();
    generator_->writeToDisk();

    read_from(tstHeader, fTstHeader, testDataPath_ + "employee.h")
    read_from(genHeader, fGenHeader, rootPath_ + sep_ + "employee.h")
    EXPECT_EQ(tstHeader, genHeader)
            << "Generated data for header must be the same with test data";

    read_from(tstSource, fTstSource, testDataPath_ + "employee.cpp")
    read_from(genSource, fGenSource, rootPath_ +  sep_ + "employee.cpp")
    EXPECT_EQ(tstSource, genSource)
            << "Generated data for source must be the same with test data";
}

TEST_F(ProjectMaker, MakeTemplateClass)
{
    auto scopeMemory = projectDb_->addScope("memory");
    auto ptrClass    = scopeMemory->addType<entity::TemplateClass>("shared_pointer");
    auto linkToPtr   = globalScope_->addType<entity::ExtendedType>();
    linkToPtr->setTypeId(ptrClass->id());
    linkToPtr->addLinkStatus();

    auto typeT       = ptrClass->addLocaleType("Value");
    ptrClass->addTemplateParameter(typeT->id());

    auto pointerToT = ptrClass->addLocaleType<entity::ExtendedType>();
    pointerToT->setTypeId(typeT->id());
    pointerToT->addPointerStatus();

    auto cPointerToT = ptrClass->addLocaleType<entity::ExtendedType>();
    cPointerToT->setTypeId(typeT->id());
    cPointerToT->addPointerStatus();
    cPointerToT->setConstStatus(true);

    auto ctor = ptrClass->makeMethod("shared_pointer");
    ctor->setReturnTypeId(STUB_ID);
    ctor->addLhsIdentificator(entity::Explicit);
    ctor->addParameter("value", pointerToT->id())->setDefaultValue("nullptr");

    // TODO: fix test

    generator_->generate();
    generator_->writeToDisk();
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
