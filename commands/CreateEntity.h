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

#include <entity/type.h>

#include <gui/graphics/graphics_types.h>

#include "basecommand.h"

namespace commands {

    class CreateEntity : public BaseCommand
    {
    public:
        CreateEntity(entity::KindOfType entityType, const common::ID &scopeID, const QPointF &pos,
                     QUndoCommand *parent = nullptr);

    public: // QUndoCommand overridies
        void undo()    override;
        void redo()    override;
        void cleanup() override;

    private:
        bool m_CleaningRequired = false;

        entity::KindOfType m_KindOfType;
        common::ID         m_ScopeID;
        QPointF            m_Pos;

        entity::SharedType  m_Entity;
        graphics::EntityPtr m_GraphicEntity;
    };

} // namespace commands
