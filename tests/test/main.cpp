#include <gtest/gtest.h>
#include "helpers.h"
#include "TestDepthSearch.h"
#include "TestTypeMaker.h"
#include "TestRelationMaker.h"
#include "TestCodeGenerator.h"
#include <helpfunctions.h>
#include <templates.cpp>

TEST_F(CodeGenerator, Type)
{
    QString futureResult("int");
    QString code(_translator->generateCode(_int));
    ASSERT_EQ(futureResult, code);

    futureResult = "project_scope::foo_scope::Foo";
    auto scopeFoo = _projectScope->addChildScope("foo_scope");
    auto foo = scopeFoo->addType("Foo");
    code = _translator->generateCode(foo);
    ASSERT_EQ(futureResult, code);
}

TEST_F(CodeGenerator, ExtendedType)
{
    entity::SharedExtendedType type = _projectScope->addType<entity::ExtendedType>();
    type->setTypeId(_int->id());

    QString futureResult("int");
    QString code(_translator->generateCode(type));
    ASSERT_EQ(futureResult, code);

    futureResult = "const int";
    type->setConstStatus(true);
    code = _translator->generateCode(type);
    ASSERT_EQ(futureResult, code);

    futureResult = "const int &";
    type->addLinkStatus();
    code = _translator->generateCode(type);
    ASSERT_EQ(futureResult, code);

    futureResult = "const int &&";
    type->addLinkStatus();
    code = _translator->generateCode(type);
    ASSERT_EQ(futureResult, code);

    type->removeLinkStatus();
    type->removeLinkStatus();

    futureResult = "const int * const";
    type->addPointerStatus(true);
    code = _translator->generateCode(type);
    ASSERT_EQ(futureResult, code);

    type->removePointerStatus();
    type->setConstStatus(false);

    futureResult = "vector<int>";
    auto vector = _globalScope->addType("vector");
    type->setTypeId(vector->id());
    type->addTemplateParameter(_int->id());
    code = _translator->generateCode(type);
    ASSERT_EQ(futureResult, code);

    futureResult = "using Ints = vector<int>;";
    type->setName("Ints");
    code = _translator->generateCode(type, true, nullptr, nullptr, true);
    ASSERT_EQ(futureResult, code);

    futureResult = "std::set<int>";
    _projectScope->removeType(type->id());
    type = _projectScope->addType<entity::ExtendedType>();
    auto stdScope = _globalScope->addChildScope("std");
    auto set = stdScope->addType("set");
    type->setTypeId(set->id());
    type->addTemplateParameter(_int->id());
    code = _translator->generateCode(type);
    ASSERT_EQ(futureResult, code);
}

TEST_F(CodeGenerator, Field)
{
    auto field = std::make_shared<entity::Field>("Number", _int->id());
    field->setPrefix("m_");

    QString futureResult("int m_Number");
    QString code(_translator->generateCode(field));
    ASSERT_EQ(futureResult, code);

    futureResult = "static int number_";
    field->setName("number");
    field->removePrefix();
    field->setSuffix("_");
    field->addKeyword(entity::FieldStatic);
    code = _translator->generateCode(field);
    ASSERT_EQ(futureResult, code);

    futureResult = "static const int number_";
    auto type = _globalScope->addType<entity::ExtendedType>();
    type->setTypeId(_int->id());
    type->setConstStatus(true);
    field->setTypeId(type->id());
    code = _translator->generateCode(field);
    ASSERT_EQ(futureResult, code);
}

TEST_F(CodeGenerator, ClassMethod)
{
    auto method = std::make_shared<entity::ClassMethod>("calc");
    method->setConstStatus(true);

    QString futureResult("void calc() const");
    auto voidType = _globalScope->addType("void");
    method->setReturnTypeId(voidType->id());
    QString code(_translator->generateCode(method));
    ASSERT_EQ(futureResult, code);

    futureResult = "double sum(double a, double b) const";
    auto doubleType = _globalScope->addType("double");
    method->setName("sum");
    method->setReturnTypeId(doubleType->id());
    method->addParameter("a", doubleType->id());
    method->addParameter("b", doubleType->id());
    code = _translator->generateCode(method);
    ASSERT_EQ(futureResult, code);

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

    code = _translator->generateCode(method);
    ASSERT_EQ(futureResult, code);

    futureResult = "explicit Foo(const QString &name)";
    method = std::make_shared<entity::ClassMethod>("Foo");
    method->addLhsIdentificator(entity::Explicit);
    method->addParameter("name", qstrExt->id());
    code = _translator->generateCode(method);
    ASSERT_EQ(futureResult, code);

    futureResult = "virtual ps::Foo *make() = 0";
    method = std::make_shared<entity::ClassMethod>("make");
    method->setReturnTypeId(fooExt->id());
    method->setRhsIdentificator(entity::PureVirtual);
    method->addLhsIdentificator(entity::Virtual);
    code = _translator->generateCode(method);
    ASSERT_EQ(futureResult, code);

    futureResult = "ps::Foo *make() override";
    method->removeLhsIdentificator(entity::Virtual);
    method->setRhsIdentificator(entity::Override);
    code = _translator->generateCode(method);
    ASSERT_EQ(futureResult, code);
}

TEST_F(CodeGenerator, TemplateClassMethod)
{
    entity::SharedTemplateClassMethod method(std::make_shared<entity::TemplateClassMethod>("swap"));

    QString futureResult("template <>\nswap()");
    QString code(_translator->generateCode(method));
    ASSERT_EQ(futureResult, code);

    futureResult = "template <class T>\nswap()";
    auto t = method->addLocaleType("T");
    method->addTemplateParameter(t->id());
    code = _translator->generateCode(method);
    ASSERT_EQ(futureResult, code);

    futureResult = "template <class T>\nswap(T *first, T *second)";
    auto ptrT = method->addLocaleType<entity::ExtendedType>();
    ptrT->setTypeId(t->id());
    ptrT->addPointerStatus();
    method->addParameter("first", ptrT->id());
    method->addParameter("second", ptrT->id());
    code = _translator->generateCode(method);
    ASSERT_EQ(futureResult, code);

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

    code = _translator->generateCode(method);
    ASSERT_EQ(futureResult.toStdString(), code.toStdString());
}

TEST_F(CodeGenerator, Enum)
{
    auto fooEnum = _projectScope->addType<entity::Enum>("Foo");

    QString futureResult("enum Foo {};");
    QString code(_translator->generateCode(fooEnum));
    ASSERT_EQ(futureResult, code);

    futureResult = "enum class Foo {};";
    fooEnum->setStrongStatus(true);
    code = _translator->generateCode(fooEnum);
    ASSERT_EQ(futureResult, code);

    futureResult = "enum class Foo : int {};";
    fooEnum->setEnumTypeId(_int->id());
    code = _translator->generateCode(fooEnum);
    ASSERT_EQ(futureResult, code);

    futureResult = "enum class Foo : int {bar, baz};";
    fooEnum->addVariable("bar");
    fooEnum->addVariable("baz");
    code = _translator->generateCode(fooEnum);
    ASSERT_EQ(futureResult, code);

    futureResult = "enum class Foo : int {bar = 0, baz = 1};";
    code = _translator->generateCode(fooEnum, true);
    ASSERT_EQ(futureResult, code);
}

TEST_F(CodeGenerator, Union)
{
    auto fooUnion = _projectScope->addType<entity::Union>("Foo");

    QString futureResult("union Foo {};");
    QString code(_translator->generateCode(fooUnion));
    ASSERT_EQ(futureResult, code);

    futureResult = "union Foo {\n    double a;\n    int b;\n};";
    auto typeDouble = _globalScope->addType("double");
    fooUnion->addField("a", typeDouble->id());
    fooUnion->addField("b", _int->id());
    code = _translator->generateCode(fooUnion);
    ASSERT_EQ(futureResult, code);
}

TEST_F(CodeGenerator, Class)
{
    auto fooClass = _projectScope->addType<entity::Class>("Foo");

    QString futureResult("class Foo {};");
    QString code(_translator->generateCode(fooClass));
    ASSERT_EQ(futureResult, code);

    futureResult = "struct Foo {};";
    fooClass->setKind(entity::StructType);
    code = _translator->generateCode(fooClass);
    ASSERT_EQ(futureResult, code);

    futureResult = "class Foo : public Bar {};";
    fooClass->setKind(entity::ClassType);
    auto barClass = _projectScope->addType<entity::Class>("Bar");
    fooClass->addParent(barClass->id(), entity::Public);
    code = _translator->generateCode(fooClass);
    ASSERT_EQ(futureResult, code);

    futureResult = "class Foo : public Bar, protected Baz {};";
    auto bazClass = _projectScope->addType<entity::Class>("Baz");
    fooClass->addParent(bazClass->id(), entity::Protected);
    code = _translator->generateCode(fooClass);
    ASSERT_EQ(futureResult, code);

    fooClass->removeParent(barClass->id());
    fooClass->removeParent(bazClass->id());

    futureResult = QString("class Foo \n{\n"
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
    code = _translator->generateCode(fooClass);
    ASSERT_EQ(futureResult, code);

    fooClass->removeField("a");
    fooClass->removeField("b");

    futureResult = QString("class Foo \n{\n"
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
    code = _translator->generateCode(fooClass);
    ASSERT_EQ(futureResult, code);

    fooClass->removeField("a");
    fooClass->removeField("b");


    futureResult = QString("class Foo \n{\n"
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

    code = _translator->generateCode(fooClass);
    ASSERT_EQ(futureResult, code);
}

TEST_F(CodeGenerator, TemplateClass)
{
    QString futureResult = QString("template <class T>\n"
                                   "struct Node "
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

    QString code(_translator->generateCode(nodeStruct));
    ASSERT_EQ(futureResult, code);
}

TEST_F(CodeGenerator, ClassImplementation)
{
    QString futureResult("int Foo::getC() const\n"
                         "{\n"
                         "}\n");

    entity::SharedClass classFoo(_projectScope->addType<entity::Class>("Foo"));
    entity::SharedMethod getterForC(classFoo->makeMethod("getC"));
    getterForC->setConstStatus(true);
    getterForC->setReturnTypeId(_int->id());

    translator::Code code(_translator->generateClassMethodsImpl(classFoo));
    ASSERT_TRUE(code.toHeader.isEmpty()) << "Header should be empty!";
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

TEST_F(CodeGenerator, TemplateClassImplementation)
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

    // TODO: add tests for templates
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
    ASSERT_FALSE(methodsList.isEmpty()) << "First class should have a getter";
    EXPECT_TRUE(methodsList.first()->isConst()) << "Getter shoudl be const";
    EXPECT_EQ(valueType->id(), methodsList.first()->returnTypeId())
            << "Getter should have valid return tyeid ";
    auto parameter = methodsList.first()->getParameter("key");
    ASSERT_NE(parameter, nullptr) << "Getter should have parameter with name \"key\"";
    EXPECT_EQ(parameter->typeId(), stringClass->id())
            << "Parameter in getter should have a valid type";

    methodsList = _firstClass->getMethod("add" + _secondClass->name());
    ASSERT_FALSE(methodsList.isEmpty()) << "First class should have a setter";
    parameter = methodsList.first()->getParameter("src_" + _secondClass->name().toLower());
    ASSERT_NE(parameter, nullptr) << "Getter should have parameter with right name";
    EXPECT_EQ(parameter->typeId(), valueType->id())
            << "Parameter in setter should have a valid type";

    methodsList = _firstClass->getMethod("remove" + _secondClass->name());
    ASSERT_FALSE(methodsList.isEmpty()) << "First class should have a deleter";
    parameter = methodsList.first()->getParameter("key");
    ASSERT_NE(parameter, nullptr) << "Deleter should have parameter with right name";
    EXPECT_EQ(parameter->typeId(), stringClass->id())
            << "Parameter in deleter should have a valid type";

    methodsList = _firstClass->getMethod(_secondClass->name().toLower() + "s");
    ASSERT_FALSE(methodsList.isEmpty()) << "First class should have a group getter";
    EXPECT_TRUE(methodsList.first()->isConst()) << "Group getter shoudl be const";
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

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
