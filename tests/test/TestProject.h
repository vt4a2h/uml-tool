#pragma once

#include <QDir>

#include <gtest/gtest.h>

#include <types.h>
#include <project/project.h>

class Project : public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
        rootPath_.append(sep_).append("tmp");
        QDir().mkpath(rootPath_);

        project_ = std::make_shared<project::Project>("TstProject", rootPath_, errors);
    }

    virtual void TearDown() override
    {
        EXPECT_TRUE(QDir(rootPath_).removeRecursively());
    }

    QString rootPath_ = QDir::current().path();
    QChar   sep_      = QDir(rootPath_).separator();
    SharedErrorList errors = std::make_shared<QStringList>();
    project::SharedProject project_;
};
