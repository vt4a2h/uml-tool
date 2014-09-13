#pragma once

#pragma once

#include <gtest/gtest.h>

#include <QString>
#include <QDir>
#include <QTextStream>

#include "types.h"

struct RelationTestParameters
{
    db::SharedDatabase globalDb_  = std::make_shared<db::Database>("Global");
    db::SharedDatabase projectDb_ = std::make_shared<db::ProjectDatabase>("Project");

    entity::SharedScope globalScope_  = globalDb_->addScope(GLOBAL_SCOPE_ID, "");
    entity::SharedScope projectScope_ = projectDb_->addScope("Project scope");

    entity::SharedClass firstClass_  = projectScope_->addType<entity::Class>("First");
    entity::SharedClass secondClass_ = projectScope_->addType<entity::Class>("Second");
};

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

    RelationTestParameters m_Parameters; // For test relation json
};

template <class RelationType, class Parameters>
void setDb(const std::shared_ptr<RelationType> &rel, const Parameters &p)
{
    // always it set in Scope::fromJson
    rel->setGlobalDatabase(p.globalDb_.get());
    rel->setProjectDatabase(p.projectDb_.get());
}

template <class RelationType, class Parameters>
std::shared_ptr<RelationType> makeRelation(const Parameters &p)
{
    return std::make_shared<RelationType>(p.firstClass_->id(),
                                          p.secondClass_->id(),
                                          p.globalDb_.get(),
                                          p.projectDb_.get());
}
