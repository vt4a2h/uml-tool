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

TEST_F(CommandsTester, CreateEntity)
{
    auto type = entity::KindOfType::Class;
    auto scopeID = common::ID::projectScopeID();
    auto pos = QPointF(10., 20.);

    auto cmd = std::make_unique<commands::CreateEntity>(type, scopeID, pos);
    cmd->redo();

    auto entity = cmd->entity();
    ASSERT_TRUE(!!entity);
    ASSERT_EQ(entity->kindOfType(), type);
    ASSERT_EQ(entity->scopeId(), scopeID);

    auto graphicEntity = cmd->graphicsEntity();
    ASSERT_TRUE(!!graphicEntity);
    ASSERT_EQ(graphicEntity->pos(), pos);
}
