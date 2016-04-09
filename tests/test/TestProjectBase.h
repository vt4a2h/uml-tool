/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 22/02/2016.
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


#include <gtest/gtest.h>

#include <db/ProjectDatabase.h>
#include <db/database.h>

#include <project/Project.h>
#include <project/project_types.hpp>

class ProjectBase : public ::testing::Test
{
protected:

    ProjectBase() { init(); }

    void init()
    {
        m_Project = std::make_shared<project::Project>();
        // Usually special slot is used for that
        const_cast<helpers::GeneratorID&>(helpers::GeneratorID::instance()).onCurrentProjectChanged(m_Project);
        m_ProjectDb = m_Project->database();
        m_ProjectScope = m_ProjectDb->getScope(common::ID::projectScopeID());
        ASSERT_TRUE(!!m_ProjectScope);

        m_GlobalDb = std::make_shared<db::Database>("global", "../../");
        ErrorList errors;
        m_GlobalDb->load(errors);
        ASSERT_TRUE(errors.isEmpty());
        m_GlobalScope = m_GlobalDb->getScope(common::ID::globalScopeID());
        ASSERT_TRUE(!!m_GlobalScope);

        m_Project->setGlobalDatabase(m_GlobalDb);
    }

    project::SharedProject m_Project;

    db::SharedDatabase m_GlobalDb;
    db::SharedProjectDatabase m_ProjectDb;

    entity::SharedScope m_GlobalScope;
    entity::SharedScope m_ProjectScope;
};
