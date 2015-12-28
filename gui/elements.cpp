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
#include "elements.h"
#include "ui_elements.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QRectF>
#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>

namespace gui {

    static const int margin = 2;
    static const QSize elementSize(100, 50);

    Elements::Elements(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::Elements)
    {
        ui->setupUi(this);

        // Add items
        QLabel * lbl = new QLabel(this);
        QPixmap pic(elementSize);
        pic.fill(Qt::transparent);
        QPainter p;
        p.begin(&pic);
        p.setRenderHint(QPainter::Antialiasing);
        p.fillRect(pic.rect(), QBrush(Qt::green));
        p.drawRect(QRectF({0, 0}, elementSize));
        p.end();
        lbl->setPixmap(pic);
        lbl->resize(elementSize);

        ui->gridLayout->addWidget(lbl, 0, 0);
        ui->gridLayout->addWidget(new QLabel("Tst 2", this), 0, 1);
    }

    Elements::~Elements()
    {
    }

    void Elements::mousePressEvent(QMouseEvent *ev)
    {
        QLabel *child = static_cast<QLabel*>(childAt(ev->pos()));
        if (!child)
            return;

        QByteArray itemData;
        QDataStream out(&itemData, QIODevice::WriteOnly);
        out << QString("test");

        QMimeData *mimeData = new QMimeData;
        mimeData->setData(mimeDataType(), itemData);

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(*child->pixmap());
        drag->setHotSpot(ev->pos() - child->pos());

        drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);
    }

    void Elements::dropEvent(QDropEvent *de)
    {
        de->ignore();
    }

    void Elements::dragEnterEvent(QDragEnterEvent *de)
    {
        de->ignore();
    }

    void Elements::dragMoveEvent(QDragMoveEvent *de)
    {
        de->acceptProposedAction();
    }

    QString Elements::mimeDataType()
    {
        return "application/x-element_data";
    }

} // namespace gui
