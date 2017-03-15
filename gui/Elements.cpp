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
#include "Elements.h"
#include "ui_elements.h"

#include <QMap>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QRectF>
#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>

#include <entity/Class.h>
#include <entity/Enum.h>
#include <entity/Union.h>
#include <entity/TemplateClass.h>
#include <entity/ExtendedType.h>

#include <application/Settings.h>

#include "qthelpers.h"

namespace gui {

    static const QSize elementSize(120, 50);

    static void addElement(entity::KindOfType kind, QWidget &parent, QGridLayout &layout,
                           int row, int col)
    {
        // Create widget
        QLabel *lbl = new QLabel(&parent);
        lbl->setProperty(Elements::elementTypePropertyName(), uint(kind));

        // Add graphics element
        QPixmap pic(elementSize);
        pic.fill(Qt::transparent);

        // Render element
        QPainter p;
        p.begin(&pic);
        p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

        QColor color =
            App::Settings::elementColor(entity::kindOfTypeToString(kind));
        QLinearGradient gradient(elementSize.width() / 2, 0,
                                 elementSize.width() / 2, elementSize.height());
        gradient.setColorAt(0, color);
        gradient.setColorAt(1, Qt::white);
        p.fillRect(pic.rect(), QBrush(gradient));
        p.setPen(QPen(color));
        p.drawRect(QRectF({0, 0}, elementSize));

        p.setPen(Qt::black);
        p.drawText(pic.rect(), Qt::AlignCenter, entity::kindOfTypeToString(kind, false /*raw*/));
        p.end();

        // Set graphic element
        lbl->setPixmap(pic);
        lbl->resize(elementSize);

        // Add widget
        layout.addWidget(lbl, row, col);
    }

    /**
     * @brief Elements::Elements
     * @param parent
     */
    Elements::Elements(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::Elements)
    {
        ui->setupUi(this);

        // Add items
        addElement(entity::KindOfType::Class        , *this, *ui->gridLayout, 0, 0);
        addElement(entity::KindOfType::Enum         , *this, *ui->gridLayout, 0, 1);
        addElement(entity::KindOfType::Union        , *this, *ui->gridLayout, 1, 0);
        addElement(entity::KindOfType::TemplateClass, *this, *ui->gridLayout, 1, 1);
        addElement(entity::KindOfType::ExtendedType , *this, *ui->gridLayout, 2, 0);
    }

    /**
     * @brief Elements::~Elements
     */
    Elements::~Elements()
    {
    }

    /**
     * @brief Elements::mousePressEvent
     * @param ev
     */
    void Elements::mousePressEvent(QMouseEvent *ev)
    {
        QLabel *lbl = static_cast<QLabel*>(childAt(ev->pos()));
        if (!lbl)
            return;

        // Preserve element type
        QByteArray itemData;
        QDataStream out(&itemData, QIODevice::WriteOnly);
        QVariant type = lbl->property(Elements::elementTypePropertyName());
        out << type.value<uint>();

        // Set mime data
        QMimeData *mimeData = new QMimeData;
        mimeData->setData(mimeDataType(), itemData);

        // Configure drag operation
        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(*lbl->pixmap());
        drag->setHotSpot(ev->pos() - lbl->pos());

        drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);
    }

    /**
     * @brief Elements::dropEvent
     * @param de
     */
    void Elements::dropEvent(QDropEvent *de)
    {
        de->ignore();
    }

    /**
     * @brief Elements::dragEnterEvent
     * @param de
     */
    void Elements::dragEnterEvent(QDragEnterEvent *de)
    {
        de->ignore();
    }

    /**
     * @brief Elements::dragMoveEvent
     * @param de
     */
    void Elements::dragMoveEvent(QDragMoveEvent *de)
    {
        de->acceptProposedAction();
    }

    /**
     * @brief Elements::mimeDataType
     * @return
     */
    QString Elements::mimeDataType()
    {
        return "application/x-element_data";
    }

    /**
     * @brief Elements::elementTypePropertyName
     * @return
     */
    const char *Elements::elementTypePropertyName()
    {
        return "elementType";
    }

} // namespace gui
