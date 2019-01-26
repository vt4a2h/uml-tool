/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 15/09/2016.
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

#include <QGraphicsScene>
#include <QMainWindow>
#include <QMenu>
#include <QDir>

#include <Entity/EntityFactory.h>
#include <Entity/Class.h>

#include <Models/ApplicationModel.h>

#include <GUI/graphics/GraphicsTypes.h>
#include <GUI/graphics/Scene.h>

#include <Commands/CommandsTypes.h>

#include <Project/ProjectDB.hpp>

#include "TestProjectBase.h"

class CommandsTester : public ProjectBase
{
protected:
    CommandsTester()
    {
    }

    void SetUp() override
    {
        m_FakeAppModel = std::make_shared<Models::ApplicationModel>();
        m_FakeAppModel->setGlobalDatabse(m_GlobalDb);
        m_FakeAppModel->projectsDb().addProject(m_Project);
        m_FakeAppModel->setCurrentProject(m_Project->name());

        m_CommandsStack = std::make_shared<QUndoStack>();
        m_Scene = std::make_unique<Graphics::Scene>(m_CommandsStack);

        const_cast<Entity::EntityFactory &>(
            Entity::EntityFactory::instance()).onSceneChanged(m_Scene.get());
    }

    void TearDown() override
    {
        m_FakeAppModel.reset();
        m_CommandsStack.reset();
        m_Scene.reset();

        m_RecentProjectsMenu.clear();
    }

    Models::SharedApplicationModel m_FakeAppModel;
    Commands::SharedCommandStack m_CommandsStack;
    Graphics::UniqueScene m_Scene;

    QMainWindow m_MainWindow;
    QMenu m_RecentProjectsMenu;
};
