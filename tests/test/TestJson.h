/*****************************************************************************
**
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 23/10/2014.
**
** This file is part of Q-UML (UML tool for Qt).
**
** Q-UML is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Q-UML is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.

** You should have received a copy of the GNU Lesser General Public License
** along with Q-UML.  If not, see <http://www.gnu.org/licenses/>.
**
*****************************************************************************/
#pragma once

#include <QString>
#include <QDir>
#include <QTextStream>

#include <gtest/gtest.h>

#include <db/database.h>
#include <db/projectdatabase.h>

#include <entity/scope.h>
#include <entity/type.h>
#include <entity/class.h>
#include <entity/classmethod.h>
#include <entity/field.h>
#include <entity/property.h>

#include <relationship/node.h>
#include <relationship/relation.h>
#include <relationship/generalization.h>
#include <relationship/dependency.h>
#include <relationship/association.h>
#include <relationship/multiplyassociation.h>
#include <relationship/realization.h>

#include <types.h>

#include <constants.h>

struct RelationTestParameters
{
    db::SharedDatabase globalDb_  = std::make_shared<db::Database>("Global");
    db::SharedDatabase projectDb_ = std::make_shared<db::ProjectDatabase>("Project");

    entity::SharedScope globalScope_;
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

        m_Parameters.globalScope_ = std::make_shared<entity::Scope>();
        m_Parameters.globalScope_->setId(entity::EntityID::globalScopeID());
        m_Parameters.projectDb_->addExistsScope(m_Parameters.globalScope_);

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
