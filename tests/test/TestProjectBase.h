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
#include <project/ProjectTypes.hpp>

#include <entity/EntityFactory.h>
#include <relationship/RelationFactory.h>

#include <helpers/GeneratorID.h>

class ProjectBase : public ::testing::Test
{
protected:

    ProjectBase() { init(); }

    void init()
    {
        m_GlobalDb = std::make_shared<db::Database>("global", "../../");

        initFactory(entity::EntityFactory::instance());
        initFactory(relationship::RelationFactory::instance());

        ErrorList errors;
        m_GlobalDb->load(errors);
        Q_ASSERT(errors.isEmpty());

        m_GlobalScope = m_GlobalDb->scope(common::ID::globalScopeID());
        ASSERT_TRUE(!!m_GlobalScope);

        setProject(std::make_shared<project::Project>());
    }

    void initFactory(const common::ElementsFactory &factory)
    {
        auto &&ef = const_cast<common::ElementsFactory &>(factory);
        ef.setGlobalDatabase(m_GlobalDb);
        ef.onProjectChanged(nullptr, m_Project);
        ef.setTreeModel(m_fakeTreeModel);
    }

    void setProject(const project::SharedProject &pr)
    {
        m_Project = pr;

        // Usually special slot is used for that
        const_cast<helpers::GeneratorID&>(
            helpers::GeneratorID::instance()).onCurrentProjectChanged(nullptr, m_Project);
        m_ProjectDb = m_Project->database();

        m_ProjectScope = m_ProjectDb->scope(common::ID::projectScopeID());
        ASSERT_TRUE(!!m_ProjectScope);

        m_Project->setGlobalDatabase(m_GlobalDb);

        initFactory(entity::EntityFactory::instance());
        initFactory(relationship::RelationFactory::instance());
    }

    db::SharedDatabase m_GlobalDb;

    project::SharedProject m_Project;
    db::SharedProjectDatabase m_ProjectDb;

    entity::SharedScope m_GlobalScope;
    entity::SharedScope m_ProjectScope;

    std::shared_ptr<models::ProjectTreeModel> m_fakeTreeModel =
        std::make_shared<models::ProjectTreeModel>();
};
