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
#include <db/Database.h>

#include <project/Project.h>
#include <project/project_types.hpp>

#include <entity/EntityFactory.h>
#include <relationship/RelationFactory.h>

class ProjectBase : public ::testing::Test
{
protected:

    ProjectBase() { init(); }

    void init()
    {
        initFactory(entity::EntityFactory::instance());
        initFactory(relationship::RelationFactory::instance());

        ErrorList errors;
        m_GlobalDb->load(errors); // TODO: add keys for graphics data to types to fix loading

        m_Project =
        // Usually special slot is used for that
        const_cast<helpers::GeneratorID&>(helpers::GeneratorID::instance()).onCurrentProjectChanged(m_Project);
        m_ProjectDb = m_Project->database();
        m_ProjectScope = m_ProjectDb->getScope(common::ID::projectScopeID());
        ASSERT_TRUE(!!m_ProjectScope);

        ASSERT_TRUE(errors.isEmpty());
        m_GlobalScope = m_GlobalDb->getScope(common::ID::globalScopeID());
        ASSERT_TRUE(!!m_GlobalScope);

        m_Project->setGlobalDatabase(m_GlobalDb);
    }

    void initFactory(const common::ElementsFactory &factory)
    {
        auto &&ef = const_cast<common::ElementsFactory &>(factory);
        ef.setGlobalDatabase(m_GlobalDb);
        ef.onProjectChanged(m_Project);
        ef.setTreeModel(m_fakeTreeModel);
    }

    db::SharedDatabase m_GlobalDb = std::make_shared<db::Database>("global", "../../");

    project::SharedProject m_Project = std::make_shared<project::Project>();
    db::SharedProjectDatabase m_ProjectDb;

    entity::SharedScope m_GlobalScope;
    entity::SharedScope m_ProjectScope;

    std::shared_ptr<models::ProjectTreeModel> m_fakeTreeModel =
        std::make_shared<models::ProjectTreeModel>();
};
