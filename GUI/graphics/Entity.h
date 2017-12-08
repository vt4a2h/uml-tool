/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 09/03/2015.
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

#include <memory>

#include <QGraphicsObject>
#include <QPointer>

#include <Entity/entity_types.hpp>

#include <Project/ProjectTypes.hpp>

#include "Common.h"

QT_BEGIN_NAMESPACE
class QGraphicsProxyWidget;
class QLineEdit;
QT_END_NAMESPACE

namespace GUI
{
    class EditEntityDialog;
}

/// namespace graphics
namespace Graphics {

    /// The Entity class
    class GraphisEntity : public QGraphicsObject
    {
        Q_OBJECT

    public:
        GraphisEntity(const Entity::SharedType &type, QGraphicsItem *parent = nullptr);
        ~GraphisEntity();

        QRectF boundingRect() const override;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget = nullptr) override;
        QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

        enum { Type = UserType + int(ElementType::Entity) };
        int type() const override;

        Entity::SharedType typeObject() const;
        void setTypeObject(const Entity::SharedType &type);

        Common::ID id() const;

        void setSelectedToConnect(bool status);
        bool selectedToConnect() const;

        static qreal rectMargin();
        QRectF frameRect() const;

    signals:
        void moved(const QPointF &from, const QPointF &to);
        void positionChanged(const QPointF &from, const QPointF &to);
        void heightChanged(qreal height);
        void widthChanged(qreal width);
        void sizeChanged();

    public slots:
        void redraw();

    private:
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
        void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

        QPolygonF resizeCorner() const;
        QRectF resizeCornerRect() const;
        QRectF headerRect() const;

        qreal width() const;
        void setWidth(qreal newWidth);

        qreal height() const;
        void setHeight(qreal newHeight);

        QPointF dataPos() const;

        QColor typeColor() const;

        QGraphicsProxyWidget *nameEditor();
        QLineEdit *nameEditorWgt();

        void connect(bool connect = true);

        void drawHeader(QPainter * painter);
        void drawFrame(QPainter * painter);
        void drawResizeCorner(QPainter * painter);
        void drawSections(QPainter * painter);
        void drawConnectFrame(QPainter * painter);

        Entity::SharedType m_Type;
        QPointF m_LastPos;
        bool m_ResizeMode;
        bool m_selectedToConnect;

        QPointer<QGraphicsProxyWidget> m_NameEditor;
    };

} // grpahics
