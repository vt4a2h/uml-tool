#pragma once

#pragma once

#include <gtest/gtest.h>

#include <QString>
#include <QDir>
#include <QTextStream>

#include "types.h"

class FileJson : public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
        m_RootPath = QDir::current().path();
        m_Sep = QDir(m_RootPath).separator();
        m_RootPath.append(m_Sep).append("tmp");
        QDir().mkpath(m_RootPath);

        EXPECT_TRUE(QFileInfo(m_RootPath).isDir() && QFileInfo(m_RootPath).isWritable())
                << "We need a writable directory";

        m_JsonFileName = m_RootPath + m_Sep + "out.json";
    }

    virtual void TearDown() override
    {
        EXPECT_TRUE(QDir(m_RootPath).removeRecursively());
    }

    QString m_RootPath;
    QString m_JsonFileName;
    SharedErrorList m_Errors = std::make_shared<ErrorList>();
    QChar m_Sep;
};
