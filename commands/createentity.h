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

#include "enums.h"
#include "types.h"
#include "basecommand.h"

class QGraphicsItem;
class QGraphicsScene;

namespace commands {

    /// The class CreateEntity
    class CreateEntity : public BaseCommand
    {
    public:
        CreateEntity(const models::SharedApplicationModal &model, entity::UserType type,
                     const QString &scopeID, QGraphicsScene &scene, const QPointF &pos,
                     QUndoCommand *parent = nullptr);

        void redo() override;
        void undo() override;

    private:
        models::SharedApplicationModal m_Model;
        entity::UserType m_Type;
        QString m_ProjectID;
        QString m_ScopeID;
        entity::SharedType m_TypeItem;
        QPointF m_Pos;
        QGraphicsItem  * m_Item;
        QGraphicsScene & m_Scene;
    };

} // namespace commands
