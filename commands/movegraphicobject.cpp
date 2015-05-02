/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 29/03/2015.
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
#include "movegraphicobject.h"

#include <QGraphicsItem>

namespace commands {

    /**
     * @brief MoveGraphicObject::MoveGraphicObject
     * @param object
     * @param name
     * @param from
     * @param to
     * @param parent
     */
    MoveGraphicObject::MoveGraphicObject(QGraphicsItem &object, const QString &name, const QPointF &from,
                                         const QPointF &to, QUndoCommand *parent)
        : BaseCommand(tr("Move object \"%1\"").arg(name), parent)
        , m_Object(object)
        , m_From(from)
        , m_To(to)
    {
    }

    /**
     * @brief MoveGraphicObject::redo
     */
    void MoveGraphicObject::redo()
    {
        m_Object.setPos(m_To);
    }

    /**
     * @brief MoveGraphicObject::undo
     */
    void MoveGraphicObject::undo()
    {
        m_Object.setPos(m_From);
    }

    /**
     * @brief MoveGraphicObject::updateScene
     */
    bool MoveGraphicObject::updateScene() const
    {
        return false;
    }

    /**
     * @brief MoveGraphicObject::updateModel
     */
    bool MoveGraphicObject::updateModel() const
    {
        return false;
    }

} // namespace commands
