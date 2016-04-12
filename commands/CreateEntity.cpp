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

#include <QGraphicsScene>

#include <db/ProjectDatabase.h>

#include <entity/Type.h>
#include <entity/EntityFactory.h>
#include <entity/scope.h>

#include <models/ProjectTreeModel.h>
#include <project/Project.h>

#include "qthelpers.h"

namespace commands {

    namespace {
        const QMap<entity::KindOfType, QString> hashName = {
            { entity::KindOfType::Type         , BaseCommand::tr("type")           },
            { entity::KindOfType::Class        , BaseCommand::tr("class")          },
            { entity::KindOfType::Enum         , BaseCommand::tr("enum")           },
            { entity::KindOfType::Union        , BaseCommand::tr("union")          },
            { entity::KindOfType::ExtendedType , BaseCommand::tr("type alias")     },
            { entity::KindOfType::TemplateClass, BaseCommand::tr("template class") },
        };
    }

    /**
     * @brief CreateEntity::CreateEntity
     * @param entityType
     * @param scopeID
     * @param pos
     * @param parent
     */
    CreateEntity::CreateEntity(entity::KindOfType entityType, const common::ID &scopeID,
                               const QPointF &pos, QUndoCommand *parent)
        : BaseCommand(tr("Add %1").arg(hashName[entityType]), parent)
        , m_KindOfType(entityType)
        , m_ScopeID(scopeID)
        , m_Pos(pos)
    {
    }

    /**
     * @brief CreateEntity::undo
     */
    void CreateEntity::undo()
    {
        // TODO: move to the RemoveEntity command (when it'll be created)
        auto &&factory = entity::EntityFactory::instance();

        if (auto &&scene = G_ASSERT(factory.scene()))
            scene->removeItem(m_GraphicEntity);

        if (auto &&db = G_ASSERT(factory.db()))
            if (auto && scope = G_ASSERT(db->getScope(m_ScopeID)))
                scope->removeType(m_Entity->id());

        if (auto &&tm = G_ASSERT(factory.treeModel()))
            if (auto &&p = G_ASSERT(factory.project()))
                tm->removeType(p->name(), m_ScopeID, m_Entity->id());
    }

    /**
     * @brief CreateEntity::redo
     */
    void CreateEntity::redo()
    {
        if (m_Done) {

        } else {
            auto &&factory = entity::EntityFactory::instance();

            m_Entity = G_ASSERT(factory.make(m_KindOfType, m_Pos, m_ScopeID));

            auto project = G_ASSERT(factory.project());
            m_GraphicEntity = G_ASSERT(G_ASSERT(project->database())->graphicsEntity(m_Entity->id()));

            m_Done = true;
        }

        m_CleaningRequired = false;
    }

    /**
     * @brief CreateEntity::cleanup
     */
    void CreateEntity::cleanup()
    {
        if (m_CleaningRequired && m_GraphicEntity)
            delete m_GraphicEntity;
    }

} // namespace commands


