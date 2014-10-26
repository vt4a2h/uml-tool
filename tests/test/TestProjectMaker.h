#pragma once

#include <gtest/gtest.h>

#include <types.h>

#include <db/database.h>
#include <generator/virtualdirectory.h>
#include <generator/basiccppprojectgenerator.h>

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

    entity::SharedScope globalScope_   = globalDb_->addScope();
    entity::SharedScope standartScope_ = globalDb_->addScope("std");

    entity::SharedType int_     = globalScope_->addType("int");
    entity::SharedType bool_    = globalScope_->addType("bool");
    entity::SharedType double_  = globalScope_->addType("double");
    entity::SharedType uint_    = globalScope_->addType("uint");
    entity::SharedType void_    = globalScope_->addExistsType(
        std::make_shared<entity::Type>("void", "", VOID_ID)
    );
    entity::SharedType nullptr_ = globalScope_->addType("nullptr");
    entity::SharedType string_  = standartScope_->addType("string");

    generator::SharedGenerator generator_;

    // file section
    QString rootPath_     = QDir::current().path();
    QChar   sep_          = QDir(rootPath_).separator();
    QString testDataPath_ = QString("..%1test_data%1").arg(sep_
                                                           );
};
