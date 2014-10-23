#pragma once

#include <gtest/gtest.h>

#include <QString>
#include <QDir>
#include <QTextStream>

#include "virtualdirectory.h"

class FileMaker : public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
        m_RootPath = QDir::current().path();
        m_Sep = QDir(m_RootPath).separator();
        m_RootPath.append(m_Sep).append("tmp");
        QDir().mkpath(m_RootPath);

        EXPECT_TRUE(QFileInfo(m_RootPath).isDir() && QFileInfo(m_RootPath).isWritable());

        m_Directory.setErrorList(m_Errors);
    }

    virtual void TearDown() override
    {
        EXPECT_TRUE(QDir(m_RootPath).removeRecursively());
    }

    QString m_RootPath;
    generator::VirtualDirectory m_Directory;
    SharedErrorList m_Errors = std::make_shared<ErrorList>();
    QChar m_Sep;
};

