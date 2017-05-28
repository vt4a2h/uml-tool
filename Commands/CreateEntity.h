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
#pragma once

#include <QPointF>

#include <Entity/Type.h>

#include <GUI/graphics/GraphicsTypes.h>

#include <Models/ModelsTypes.hpp>

#include "BaseCommand.h"

namespace Commands {

    class CreateEntity : public BaseCommand
    {
    public:
        CreateEntity(Entity::KindOfType entityType, const Common::ID &scopeID, const QPointF &pos,
                     QUndoCommand *parent = nullptr);

        Entity::SharedType entity() const;
        Graphics::EntityPtr graphicsEntity() const;

    public: // QUndoCommand overridies
        void undo()    override;
        void redo()    override;

    public: // BaseCommand overridies
        void sanityCheck() override;
        void cleanup() override;

    private:
        Entity::KindOfType m_KindOfType;
        Common::ID         m_ScopeID;
        QPointF            m_Pos;

        Entity::SharedType  m_Entity;
        Graphics::EntityPtr m_GraphicEntity;

        // Aux
        Entity::SharedScope m_Scope;
        Graphics::ScenePtr m_Scene;
        Models::SharedTreeModel m_TreeModel;
        QString m_ProjectName;
    };

} // namespace Commands
