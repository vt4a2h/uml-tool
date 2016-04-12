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
#include <db/ProjectDatabase.h>

#include <entity/scope.h>
#include <entity/Type.h>
#include <entity/Class.h>
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

#include "TestProjectBase.h"

class FileJson : public ProjectBase
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

        m_FirstClass  = m_ProjectScope->addType<entity::Class>("First");
        m_SecondClass = m_ProjectScope->addType<entity::Class>("Second");
    }

    virtual void TearDown() override
    {
        EXPECT_TRUE(QDir(m_RootPath).removeRecursively());
    }

    QString m_RootPath;
    QString m_JsonFileName;
    SharedErrorList m_Errors = std::make_shared<ErrorList>();
    QChar m_Sep;

    entity::SharedClass m_FirstClass;
    entity::SharedClass m_SecondClass;
};
