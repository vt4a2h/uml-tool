/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 28/12/2015.
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
#include "view.h"

#include <QDropEvent>
#include <QByteArray>
#include <QMimeData>
#include <QDebug>

#include "elements.h"

namespace gui {

    View::View(QWidget *parent)
        : QGraphicsView(parent)
    {
        setAcceptDrops(true);
    }

    void View::dropEvent(QDropEvent *de)
    {
        qDebug() << "drop";
        if (de->mimeData()->hasFormat(Elements::mimeDataType())) {
            QByteArray itemData = de->mimeData()->data(Elements::mimeDataType());
            QDataStream in(&itemData, QIODevice::ReadOnly);

            QString msg;
            in >> msg;

            qDebug() << msg;

//            if (de->source() == this) {
//                de->setDropAction(Qt::MoveAction);
//                de->accept();
//            } else {
                de->acceptProposedAction();
//            }
        }
    }

    void View::dragEnterEvent(QDragEnterEvent *de)
    {
        qDebug() << "drag enter";
        if (de->mimeData()->hasFormat(Elements::mimeDataType())) {
//            if (de->source() == this) {
//                de->setDropAction(Qt::MoveAction);
//                de->accept();
//            } else {
                de->acceptProposedAction();
//            }
        } else {
            de->ignore();
        }
    }

    void View::dragMoveEvent(QDragMoveEvent *de)
    {
        qDebug() << "drag move";
        if (de->mimeData()->hasFormat(Elements::mimeDataType())) {
//            if (de->source() == this) {
//                de->setDropAction(Qt::MoveAction);
//                de->accept();
//            } else {
                de->acceptProposedAction();
//            }
        } else {
            de->ignore();
        }
    }

    void View::dragLeaveEvent(QDragLeaveEvent *de)
    {
        de->ignore();
    }

} // namespace gui

