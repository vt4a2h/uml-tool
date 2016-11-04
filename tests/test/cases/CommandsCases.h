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

#include <commands/CreateEntity.h>
#include <commands/CreateScope.h>
#include <commands/MakeProjectCurrent.h>
#include <commands/MoveGraphicObject.h>
#include <commands/MoveTypeToAnotherScope.h>
#include <commands/RemoveProject.h>

TEST_F(CommandsTester, CreateEntityCommand)
{
    auto scopeID = common::ID::projectScopeID();
    auto pos = QPointF(10., 20.);

    for (uint type = uint(entity::KindOfType::Type);
         type <= uint(entity::KindOfType::TemplateClass);
         ++type) {
        auto kindOfType = entity::KindOfType(type);

        auto cmd = std::make_unique<commands::CreateEntity>(kindOfType, scopeID, pos);
        cmd->redo();

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

        cmd->undo();
        ASSERT_TRUE(!!entity);
        ASSERT_FALSE(!!m_ProjectDb->scope(scopeID)->type(entity->id()));
        ASSERT_TRUE(!!graphicEntity);
        ASSERT_FALSE(!!m_ProjectDb->graphicsEntity(entity->id()));
    }
}

TEST_F(CommandsTester, CreateScopeCommand)
{
    QString scopeName = "Foo";

    auto cmd = std::make_unique<commands::CreateScope>(scopeName, *m_FakeAppModel);

    cmd->redo();
    ASSERT_FALSE(cmd->scopeName().isEmpty());
    ASSERT_TRUE(!!cmd->scope());

    auto newScope = m_ProjectDb->scope(cmd->scope()->id());
    ASSERT_EQ(newScope, cmd->scope());

    cmd->undo();
    ASSERT_FALSE(!!m_ProjectDb->scope(newScope->id()));

    cmd->redo();
    ASSERT_TRUE(!!m_ProjectDb->scope(newScope->id()));
}

TEST_F(CommandsTester, MakeProjectCurrent)
{
    QString testProjectName = "Test Project";
    auto testProject = m_FakeAppModel->makeProject(testProjectName, "");
    auto oldCurrent = m_FakeAppModel->currentProject();
    ASSERT_TRUE(!!oldCurrent);

    ASSERT_FALSE(m_FakeAppModel->currentProject() == testProject);
    auto cmd = std::make_unique<commands::MakeProjectCurrent>(
                   testProjectName, m_FakeAppModel, graphics::ScenePtr());

    cmd->redo();

    ASSERT_EQ(testProject, m_FakeAppModel->currentProject());
    ASSERT_EQ(oldCurrent->name(), cmd->previousProjectName());
    ASSERT_EQ(testProjectName, cmd->currentProjectName());

    cmd->undo();
    ASSERT_EQ(m_FakeAppModel->project(cmd->previousProjectName()), m_FakeAppModel->currentProject());

    cmd->redo();
    ASSERT_EQ(testProject, m_FakeAppModel->currentProject());
}

TEST_F(CommandsTester, MoveGraphicObject)
{
    const QPointF initialPos {42., 123.};
    const QPointF dstPoint   {100., 100.};
    entity::SharedType someType = std::make_shared<entity::Type>();

    graphics::Entity e(someType);
    e.setPos(initialPos);
    ASSERT_EQ(e.pos(), initialPos);

    auto cmd =
        std::make_unique<commands::MoveGraphicObject>(e, someType->name(), initialPos, dstPoint);

    cmd->redo();
    ASSERT_EQ(e.pos(), dstPoint);

    cmd->undo();
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

    auto cmd = std::make_unique<commands::MoveTypeToAnotherScope>(type, m_FakeAppModel, firstScope,
                                                                secondScope);

    cmd->redo();
    ASSERT_FALSE(firstScope->containsType(type->id()));
    ASSERT_TRUE(secondScope->containsType(type->id()));

    cmd->undo();
    ASSERT_TRUE(firstScope->containsType(type->id()));
    ASSERT_FALSE(secondScope->containsType(type->id()));
}

TEST_F(CommandsTester, RemoveProject)
{
    auto createEntityCmd = std::make_unique<commands::CreateEntity>(
        entity::KindOfType::Class, common::ID::projectScopeID(), QPointF(10, 20));
    createEntityCmd->redo();

    auto removeProject =
        std::make_unique<commands::RemoveProject>(m_Project, m_FakeAppModel, m_Scene.get());

    removeProject->redo();
    ASSERT_FALSE(!!m_FakeAppModel->project(m_Project->name()));
    ASSERT_FALSE(m_Scene->items().contains(createEntityCmd->graphicsEntity().data()));

    removeProject->undo();
    ASSERT_TRUE(!!m_FakeAppModel->project(m_Project->name()));
    ASSERT_EQ(m_FakeAppModel->currentProject(), m_Project);
    ASSERT_TRUE(m_Scene->items().contains(createEntityCmd->graphicsEntity().data()));
}
