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

#include "qthelpers.h"

namespace gui {

    struct ElemntAttributes
    {
        QString name;
        QColor color;
    };

    static const QSize elementSize(120, 50);
    static const QMap<SchemeElements, ElemntAttributes> elAttributes =
        { {SchemeElements::Class,         {"Class",          {245, 224, 119}}},
          {SchemeElements::Enum,          {"Enumeration",    {83 , 185, 86 }}},
          {SchemeElements::Union,         {"Union",          {144, 199, 229}}},
          {SchemeElements::TemplateClass, {"Template class", {152, 131, 190}}},
          {SchemeElements::Alias,         {"Alias",          {249, 181, 194}}},
        };

    static void addElement(SchemeElements type, QWidget &parent, QGridLayout &layout,
                           int row, int col)
    {
        QString name = elAttributes[type].name;
        QColor color = elAttributes[type].color;

        // Create widget
        QLabel * lbl = new QLabel(&parent);
        lbl->setProperty(Elements::elementTypePropertyName(), uint(type));

        // Add graphics element
        QPixmap pic(elementSize);
        pic.fill(Qt::transparent);

        // Render element
        QPainter p;
        p.begin(&pic);
        p.setRenderHint(QPainter::Antialiasing);

        QLinearGradient gradient(elementSize.width() / 2, 0,
                                 elementSize.width() / 2, elementSize.height());
        gradient.setColorAt(0, color);
        gradient.setColorAt(1, Qt::white);
        p.fillRect(pic.rect(), QBrush(gradient));
        p.setPen(QPen(color));
        p.drawRect(QRectF({0, 0}, elementSize));

        p.setPen(Qt::black);
        p.drawText(elementSize.width() / 2 - p.fontMetrics().width(name) / 2,
                   elementSize.height() / 2, name);
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
        addElement(SchemeElements::Class, *this, *ui->gridLayout, 0, 0);
        addElement(SchemeElements::Enum, *this, *ui->gridLayout, 0, 1);
        addElement(SchemeElements::Union, *this, *ui->gridLayout, 1, 0);
        addElement(SchemeElements::TemplateClass, *this, *ui->gridLayout, 1, 1);
        addElement(SchemeElements::Alias, *this, *ui->gridLayout, 2, 0);
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
