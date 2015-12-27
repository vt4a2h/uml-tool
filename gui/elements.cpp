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

#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QRectF>

namespace gui {

    static const int margin = 2;
    static const QSize elementSize(100, 50);

    Elements::Elements(QWidget *parent)
        : QWidget(parent)
    {
        // Add layout
        QGridLayout * layout = new QGridLayout(this);
        layout->setSpacing(margin);
        setLayout(layout);

        // Add items
        QLabel * lbl = new QLabel(this);
        QPixmap pic(elementSize);
        pic.fill(Qt::transparent);
        QPainter p;
        p.setRenderHint(QPainter::Antialiasing);
        p.begin(&pic);
        p.fillRect(pic.rect(), QBrush(Qt::green));
        p.drawRect(QRectF(0, 0, 99, 49));
        p.end();
        lbl->setPixmap(pic);
        lbl->resize(elementSize);

        layout->addWidget(lbl, 0, 0);
        layout->addWidget(new QLabel("Tst 2", this), 0, 1);
    }

} // namespace gui
