/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 07/04/2016.
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
#include "CreateEntity.h"

#include <QMap>

#include <entity/type.h>

namespace commands {

    namespace {
        const QMap<entity::KindOfType, QString> hashName = {
            { entity::KindOfType::Type         , BaseCommand::tr("type")           },
            { entity::KindOfType::Class        , BaseCommand::tr("class (struct)") },
            { entity::KindOfType::Enum         , BaseCommand::tr("enum")           },
            { entity::KindOfType::Union        , BaseCommand::tr("union")          },
            { entity::KindOfType::ExtendedType , BaseCommand::tr("type alias")     },
            { entity::KindOfType::TemplateClass, BaseCommand::tr("template class") },
        };
    }

    CreateEntity::CreateEntity(entity::KindOfType entityType, const common::ID &scopeID,
                               const QPointF &pos, QGraphicsScene &scene, QUndoCommand *parent)
        : BaseCommand(tr("Add %1").arg(hashName[entityType]), parent)
        , m_Type(entityType)
        , m_ScopeID(scopeID)
        , m_Pos(pos)
        , m_Scene(scene)
    {
    }

} // namespace commands
