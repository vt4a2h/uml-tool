/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 01/04/2015.
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
#include "createentity.h"

#include <QGraphicsScene>

#include <entity/entitiesfactory.h>
#include <entity/type.h>
#include <entity/class.h>

#include <gui/graphics/entity.h>

#include <models/applicationmodel.h>

#include <utility/helpfunctions.h>

#include "constants.cpp"

namespace commands {

    /**
     * @brief CreateEntity::CreateEntity
     * @param model
     * @param type
     * @param scopeID
     * @param scene
     * @param pos
     * @param parent
     */
    CreateEntity::CreateEntity(const models::SharedApplicationModal &model, entity::UserType type,
                               const QString &scopeID, QGraphicsScene &scene, const QPointF &pos, QUndoCommand *parent)
        : BaseCommand(tr("Add %1").arg(utility::userTypeToString(type)), parent)
        , m_Model(model)
        , m_Type(type)
        , m_ProjectID(model && model->currentProject() ? model->currentProject()->id() : STUB_ID)
        , m_ScopeID(scopeID)
        , m_TypeItem(nullptr)
        , m_Pos(pos)
        , m_Item(nullptr)
        , m_Scene(scene)
    {
    }

    /**
     * @brief CreateEntity::redo
     */
    void CreateEntity::redo()
    {
        if (m_Done) {
            m_Model->addExistsType(m_ProjectID, m_ScopeID, m_TypeItem);

            m_Scene.addItem(m_Item);
            static_cast<graphics::Entity *>(m_Item)->setTypeObject(m_TypeItem);
        } else {
            Q_ASSERT( m_Type == entity::UserClassType ); // only for classes now

            auto&& factory = entity::EntitiesFactory::get();
            // TODO: use other factory method which is not require static_cast
            m_TypeItem = std::static_pointer_cast<entity::Type>(factory.makeClass(m_Model, m_ScopeID,
                                                                                  m_Scene, m_Pos));
            m_Item = m_Scene.itemAt(m_Pos, QTransform());

            m_Done = true;
        }
    }

    /**
     * @brief CreateEntity::undo
     */
    void CreateEntity::undo()
    {
        Q_ASSERT(m_Model);
        Q_ASSERT(m_TypeItem);

        m_Scene.removeItem(m_Item);
        m_Model->removeType(m_ProjectID, m_ScopeID, m_TypeItem->id());
    }

} // namespace commands
