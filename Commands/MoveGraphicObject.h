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
#pragma once

#include <QPointF>
#include "BaseCommand.h"

class QGraphicsItem;

namespace Commands {

    /// The MoveGraphicObject class
    class MoveGraphicObject : public BaseCommand
    {
    public:
        MoveGraphicObject(QGraphicsItem &object, const QString &name,
                          const QPointF &from, const QPointF &to, QUndoCommand * parent = nullptr);

        void redoImpl() override;
        void undoImpl() override;

    private:
        QGraphicsItem &m_Object;
        QPointF m_From;
        QPointF m_To;
    };

} // // namespace Commands
