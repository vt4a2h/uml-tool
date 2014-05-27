#include <gtest/gtest.h>
#include "helpers.h"
#include "TestDepthSearch.h"
#include "TestTypeMaker.h"
#include "TestRelationMaker.h"
#include "TestProjectTranslator.h"
#include <helpfunctions.h>

TEST_F(Translator, Type)
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

TEST_F(Translator, ExtendedType)
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
    code = _translator->generateCode(type, true);
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

TEST_F(Translator, Field)
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

TEST_F(Translator, ClassMethod)
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

TEST_F(Translator, Enum)
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

TEST_F(Translator, Union)
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
