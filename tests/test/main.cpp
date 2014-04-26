#include <gtest/gtest.h>
#include "helpers.h"
#include "TestDepthSearch.h"
#include "TestTypeMaker.h"
#include "TestRelationMaker.h"

TEST_F(RelationMaker, Association)
{
    auto association = std::make_shared<relationship::Association>(_firstClass->id(),
                                                                   _secondClass->id(),
                                                                   _globalDb,
                                                                   _projectDb);

    EXPECT_EQ(association->relationType(), relationship::AssociationRelation)
            << "Association should have type relationship::Association";

    auto type = _firstProjectScope->addType<entity::ExtendedType>("PointerToFirstClass");
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
                                                                         _globalDb,
                                                                         _projectDb);

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
                                                                 _globalDb,
                                                                 _projectDb);
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

TEST_F(TestTypeMaker, MakesRightTypes)
{
    for (auto typeName : _typesNames)
        EXPECT_EQ(_types[typeName]->type(),
                  static_cast<entity::UserType>(_typesNames.indexOf(typeName))
                  ) << typeName.toStdString() << " class object sholud has valid type id";
}

TEST_F(TestDepthSearch, ScopeSearchWorks)
{
    entity::SharedScope p(nullptr);

    search_circle(_scopes, depthScopeSearch, scope)
    invalid_case(depthScopeSearch, "foobarid")
}

TEST_F(TestDepthSearch, TypeSearchWorks)
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
