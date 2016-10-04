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
        ASSERT_FALSE(!!graphicEntity);

        cmd->undo();
        ASSERT_TRUE(!!entity);
        ASSERT_FALSE(!!m_ProjectDb->scope(scopeID)->type(entity->id()));
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
