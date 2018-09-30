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

#include "TestCommands.h"

#include <Commands/CreateEntity.h>
#include <Commands/CreateScope.h>
#include <Commands/MakeProjectCurrent.h>
#include <Commands/MoveGraphicObject.h>
#include <Commands/MoveTypeToAnotherScope.h>
#include <Commands/RemoveProject.h>
#include <Commands/RenameEntity.h>
#include <Commands/OpenProject.h>

TEST_F(CommandsTester, CreateEntityCommand)
{
    auto scopeID = Common::ID::projectScopeID();
    auto pos = QPointF(10., 20.);

    for (uint type = uint(Entity::KindOfType::Type);
         type <= uint(Entity::KindOfType::TemplateClass);
         ++type) {
        auto kindOfType = Entity::KindOfType(type);

        auto cmd = std::make_unique<Commands::CreateEntity>(kindOfType, scopeID, pos);
        cmd->redoImpl();

        auto entity = cmd->entity();
        ASSERT_TRUE(!!entity);
        ASSERT_EQ(entity->kindOfType(), kindOfType);
        ASSERT_EQ(entity->scopeId(), scopeID);
        ASSERT_TRUE(!!m_ProjectDb->scope(scopeID)->type(entity->id()));

        auto graphicEntity = cmd->graphicsEntity();
        ASSERT_TRUE(!!graphicEntity);
        ASSERT_EQ(graphicEntity->typeObject(), entity);
        ASSERT_EQ(graphicEntity->id(), entity->id());
        ASSERT_TRUE(!!m_ProjectDb->graphicsEntity(entity->id()));
        ASSERT_EQ(m_ProjectDb->graphicsEntity(entity->id()), graphicEntity);

        cmd->undoImpl();
        ASSERT_TRUE(!!entity);
        ASSERT_FALSE(!!m_ProjectDb->scope(scopeID)->type(entity->id()));
        ASSERT_TRUE(!!graphicEntity);
        ASSERT_FALSE(!!m_ProjectDb->graphicsEntity(entity->id()));
    }
}

TEST_F(CommandsTester, CreateScopeCommand)
{
    QString scopeName = "Foo";

    auto cmd = std::make_unique<Commands::CreateScope>(scopeName, *m_FakeAppModel);

    cmd->redoImpl();
    ASSERT_FALSE(cmd->scopeName().isEmpty());
    ASSERT_TRUE(!!cmd->scope());

    auto newScope = m_ProjectDb->scope(cmd->scope()->id());
    ASSERT_EQ(newScope, cmd->scope());

    cmd->undoImpl();
    ASSERT_FALSE(!!m_ProjectDb->scope(newScope->id()));

    cmd->redoImpl();
    ASSERT_TRUE(!!m_ProjectDb->scope(newScope->id()));
}

TEST_F(CommandsTester, MakeProjectCurrent)
{
    QString testProjectName = "Test Project";
    auto testProject = m_FakeAppModel->makeProject(testProjectName, "");
    auto oldCurrent = m_FakeAppModel->currentProject();
    ASSERT_TRUE(!!oldCurrent);

    ASSERT_FALSE(m_FakeAppModel->currentProject() == testProject);
    auto cmd = std::make_unique<Commands::MakeProjectCurrent>(
                   testProjectName, m_FakeAppModel, Graphics::ScenePtr());

    cmd->redoImpl();

    ASSERT_EQ(testProject, m_FakeAppModel->currentProject());
    ASSERT_EQ(oldCurrent->name(), cmd->previousProjectName());
    ASSERT_EQ(testProjectName, cmd->currentProjectName());

    cmd->undoImpl();
    ASSERT_EQ(m_FakeAppModel->project(cmd->previousProjectName()), m_FakeAppModel->currentProject());

    cmd->redoImpl();
    ASSERT_EQ(testProject, m_FakeAppModel->currentProject());
}

TEST_F(CommandsTester, MoveGraphicObject)
{
    const QPointF initialPos {42., 123.};
    const QPointF dstPoint   {100., 100.};
    Entity::SharedType someType = std::make_shared<Entity::Type>();

    Graphics::GraphisEntity e(someType);
    e.setPos(initialPos);
    ASSERT_EQ(e.pos(), initialPos);

    auto cmd =
        std::make_unique<Commands::MoveGraphicObject>(e, someType->name(), initialPos, dstPoint);

    cmd->redoImpl();
    ASSERT_EQ(e.pos(), dstPoint);

    cmd->undoImpl();
    ASSERT_EQ(e.pos(), initialPos);
}

TEST_F(CommandsTester, MoveTypeToAnotherScope)
{
    auto firstScope = m_Project->database()->addScope("first");
    ASSERT_TRUE(!!firstScope);

    auto secondScope = m_Project->database()->addScope("second");
    ASSERT_TRUE(!!secondScope);

    auto type = firstScope->addType("SomeType");
    ASSERT_TRUE(!!type);
    ASSERT_TRUE(firstScope->containsType(type->id()));
    ASSERT_FALSE(secondScope->containsType(type->id()));

    auto cmd = std::make_unique<Commands::MoveTypeToAnotherScope>(type, m_FakeAppModel, firstScope,
                                                                secondScope);

    cmd->redoImpl();
    ASSERT_FALSE(firstScope->containsType(type->id()));
    ASSERT_TRUE(secondScope->containsType(type->id()));

    cmd->undoImpl();
    ASSERT_TRUE(firstScope->containsType(type->id()));
    ASSERT_FALSE(secondScope->containsType(type->id()));
}

TEST_F(CommandsTester, RemoveProject)
{
    auto createEntityCmd = std::make_unique<Commands::CreateEntity>(
        Entity::KindOfType::Class, Common::ID::projectScopeID(), QPointF(10, 20));
    createEntityCmd->redoImpl();

    auto removeProject =
        std::make_unique<Commands::RemoveProject>(m_Project, m_FakeAppModel, m_Scene.get());

    removeProject->redoImpl();
    ASSERT_FALSE(!!m_FakeAppModel->project(m_Project->name()));
    ASSERT_FALSE(m_Scene->items().contains(createEntityCmd->graphicsEntity().data()));

    removeProject->undoImpl();
    ASSERT_TRUE(!!m_FakeAppModel->project(m_Project->name()));
    ASSERT_EQ(m_FakeAppModel->currentProject(), m_Project);
    ASSERT_TRUE(m_Scene->items().contains(createEntityCmd->graphicsEntity().data()));
}

TEST_F(CommandsTester, RenameEntity)
{
    QString defaultClassName = "Foo";
    QString newClassName = "Bar";

    auto c = m_ProjectScope->addType<Entity::Class>(defaultClassName);
    ASSERT_EQ(c->name(), defaultClassName);

    auto renameEntityCommand = std::make_unique<Commands::RenameEntity>(c, newClassName);

    renameEntityCommand->redoImpl();
    ASSERT_EQ(c->name(), newClassName);

    renameEntityCommand->undoImpl();
    ASSERT_EQ(c->name(), defaultClassName);
}

TEST_F(CommandsTester, OpenProject)
{
    // TODO: reorganize tests (create project in "setup" method and release in "tear down")
    auto tstProject = std::make_shared<Projects::Project>("MyTstProject", QDir::currentPath());
    tstProject->setGlobalDatabase(m_GlobalDb);
    tstProject->save();

    auto currentProject = m_FakeAppModel->currentProject();

    auto cmd = std::make_shared<Commands::OpenProject>("Open new project", tstProject->fullPath(),
                                                       m_FakeAppModel, m_CommandsStack, m_Scene.get(),
                                                       m_MainWindow, m_RecentProjectsMenu);
    cmd->setSuppressDialogs(true);

    cmd->redoImpl();

    auto newCurrentProject = m_FakeAppModel->currentProject();
    ASSERT_TRUE(!!newCurrentProject);
    ASSERT_NE(currentProject, newCurrentProject);
    ASSERT_EQ(newCurrentProject->fullPath(), tstProject->fullPath());

    cmd->undoImpl();

    ASSERT_EQ(m_FakeAppModel->currentProject(), currentProject);
}
