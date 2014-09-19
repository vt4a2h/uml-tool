#pragma once

#include <types.h>
#include <database.h>
#include <virtualdirectory.h>
#include <basiccppprojectgenerator.h>

#include <gtest/gtest.h>

class ProjectMaker : public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
        rootPath_.append(sep_).append("tmp");
        QDir().mkpath(rootPath_);

        generator_ = std::make_shared<generator::BasicCppProjectGenerator>(globalDb_,
                                                                           projectDb_,
                                                                           rootPath_);
        generator_->setProjectName("test_app");
    }

    virtual void TearDown() override
    {
        EXPECT_TRUE(QDir(rootPath_).removeRecursively());
    }

    db::SharedDatabase globalDb_  = std::make_shared<db::Database>("Global");
    db::SharedDatabase projectDb_ = std::make_shared<db::ProjectDatabase>("Project");

    entity::SharedScope globalScope_        = globalDb_->addScope(GLOBAL_SCOPE_ID, "");
    entity::SharedScope firstProjectScope_  = projectDb_->addScope("first_scope");
    entity::SharedScope secondProjectScope_ = projectDb_->addScope("second_scope");

    entity::SharedClass firstClass_  = firstProjectScope_->addType<entity::Class>("FirstClass");
    entity::SharedClass secondClass_ = secondProjectScope_->addType<entity::Class>("SecondClass");

    entity::SharedType int_    = globalScope_->addType("int");
    entity::SharedType double_ = globalScope_->addType("double");
    entity::SharedType void_   = globalScope_->addType("void");

    generator::SharedGenerator generator_;

    // file section
    QString rootPath_ = QDir::current().path();
    QChar   sep_      = QDir(rootPath_).separator();
};
