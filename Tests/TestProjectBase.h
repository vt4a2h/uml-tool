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

#include <QDir>
#include <QUndoStack>

#include <Commands/CommandsTypes.h>

#include <DB/ProjectDatabase.h>
#include <DB/Database.h>

#include <Project/Project.h>
#include <Project/ProjectTypes.hpp>

#include <Entity/EntityFactory.h>
#include <Relationship/RelationFactory.h>

#include <Models/ProjectTreeModel.h>

#include <Helpers/GeneratorID.h>

#include "Arguments.hpp"

class ProjectBase : public ::testing::Test
{
protected:

    ProjectBase() { init(); }

    void init()
    {
        m_GlobalDb = std::make_shared<DB::Database>(
            "global", Testing::Arguments::instance().dbPath() + QDir::separator()
        );

        initFactory(Entity::EntityFactory::instance());
        initFactory(Relationship::RelationFactory::instance());

        ErrorList errors;
        m_GlobalDb->load(errors);
        Q_ASSERT(errors.isEmpty());

        m_GlobalScope = m_GlobalDb->scope(Common::ID::globalScopeID());
        ASSERT_TRUE(!!m_GlobalScope);

        setProject(std::make_shared<Projects::Project>());
    }

    void initFactory(const Common::ElementsFactory &factory)
    {
        auto &&ef = const_cast<Common::ElementsFactory &>(factory);
        ef.setGlobalDatabase(m_GlobalDb);
        ef.onProjectChanged(nullptr, m_Project);
        ef.setTreeModel(m_fakeTreeModel);
        ef.setCommandStack(m_tmpCommandStack);
    }

    void setProject(const Projects::SharedProject &pr)
    {
        m_Project = pr;

        // Usually special slot is used for that
        const_cast<Helpers::GeneratorID&>(
            Helpers::GeneratorID::instance()).onCurrentProjectChanged(nullptr, m_Project);
        m_ProjectDb = m_Project->database();

        m_ProjectScope = m_ProjectDb->scope(Common::ID::projectScopeID());
        ASSERT_TRUE(!!m_ProjectScope);

        m_Project->setGlobalDatabase(m_GlobalDb);

        initFactory(Entity::EntityFactory::instance());
        initFactory(Relationship::RelationFactory::instance());
    }

    DB::SharedDatabase m_GlobalDb;

    Projects::SharedProject m_Project;
    DB::SharedProjectDatabase m_ProjectDb;

    Entity::SharedScope m_GlobalScope;
    Entity::SharedScope m_ProjectScope;

    std::shared_ptr<Models::ProjectTreeModel> m_fakeTreeModel =
        std::make_shared<Models::ProjectTreeModel>();

    Commands::SharedCommandStack m_tmpCommandStack = std::make_shared<QUndoStack>();
};
