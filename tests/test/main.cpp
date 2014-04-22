#include <gtest/gtest.h>
#include "TestDepthSearch.h"

TEST_F(TestDepthSearch, ScopeSearchWorks)
{
    auto p = _d->depthScopeSearch(sc1->id());

    EXPECT_TRUE(p.operator bool())
            << "depthScopeSearch() should return valid ptr for sc1 object";
    EXPECT_EQ(sc1, p) << "depthScopeSearch() should find sc1 object";

    p = _d->depthScopeSearch(sc2->id());
    EXPECT_TRUE(p.operator bool())
            << "depthScopeSearch() should return valid ptr for sc2 object";
    EXPECT_EQ(sc2, p) << "depthScopeSearch() should find sc2 object";

    p = _d->depthScopeSearch(sc3->id());
    EXPECT_TRUE(p.operator bool())
            << "depthScopeSearch() should return valid ptr for sc3 object";
    EXPECT_EQ(sc3, p) << "depthScopeSearch() should find sc3 object";

    p = _d->depthScopeSearch(sc4->id());
    EXPECT_TRUE(p.operator bool())
            << "depthScopeSearch() should return valid ptr for sc4 object";
    EXPECT_EQ(sc4, p) << "depthScopeSearch() should find sc4 object";

    p = _d->depthScopeSearch(sc5->id());
    EXPECT_TRUE(p.operator bool())
            << "depthScopeSearch() should return valid ptr for sc5 object";
    EXPECT_EQ(sc5, p) << "depthScopeSearch() should find sc5 object";

    p = _d->depthScopeSearch("foobarid");
    EXPECT_FALSE(p.operator bool())
            << "depthScopeSearch() should be null";
    EXPECT_EQ(p, nullptr) << "depthScopeSearch() should return nullptr for invalid id";
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
