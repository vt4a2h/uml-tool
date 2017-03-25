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
#include "Entity.h"

#include <QPainter>
#include <QEvent>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsScene>

#include <application/Settings.h>

#include <gui/EditEntityDialog.h>

#include <entity/Type.h>
#include <entity/Enum.h>
#include <entity/Union.h>
#include <entity/Class.h>
#include <entity/TemplateClass.h>
#include <entity/ExtendedType.h>
#include <entity/Scope.h>
#include <entity/ClassMethod.h>
#include <entity/field.h>

#include <utility/helpfunctions.h>

#include <entity/Property.h>
#include <entity/GraphicEntityData.h>

#include "Constants.h"
#include "QtHelpers.h"

namespace graphics {

    namespace {
        constexpr qreal margin           = 2  ;
        constexpr qreal lineIndentFactor = 2  ;
        constexpr qreal minimumHeight    = 30 ;
        constexpr qreal lineHeight       = 20 ;
        constexpr qreal minimumWidth     = 120;
        constexpr qreal paddingPercent   = 0.1;

        const QSizeF resizeCornerSize(12, 12);
        const Qt::CursorShape defaultCursorShape = Qt::ArrowCursor;

        /// Cuts too long text
        QString cutText(const QString &text, const QFontMetrics &fm, qreal frameWidth)
        {
            static const QString rightSymbols("...");

            QString result(text);
            QSize textSize(fm.size(Qt::TextSingleLine, text));
            int maxTextWidth = frameWidth * (1 - paddingPercent);
            if (textSize.width() > maxTextWidth) {
                int symbolsCount = maxTextWidth / (textSize.width() / result.count());
                symbolsCount -= rightSymbols.count();
                result.chop(result.count() - symbolsCount);
                result.append(rightSymbols);
            }

            return result;
        }

        bool drawSectionText(QPainter * painter, const QString &text, qreal width, int flags,
                             QPointF &topLeft, qreal &availableHeight)
        {
            if (qFuzzyCompare(availableHeight, 0.))
                return false;

            qreal currentLineHeight = availableHeight < lineHeight ? availableHeight : lineHeight;

            QPointF textRectLeft(topLeft + QPointF(lineIndentFactor * margin, 0));
            QRectF textRect(textRectLeft, QSizeF(width, currentLineHeight));
            painter->drawText(textRect, flags, text);

            availableHeight -= currentLineHeight;
            topLeft.ry() += currentLineHeight;

            return true;
        }

        template <class Container>
        void drawSection(QPainter * painter, const QString &sectionName, const Container elements,
                         QPointF &topLeft, qreal &availableHeight, qreal width,
                         const QColor &frameColor)
        {
            if (qFuzzyCompare(availableHeight, 0.) || elements.isEmpty())
                return;

            painter->save();

            painter->setRenderHint(QPainter::TextAntialiasing);

            // Draw section name
            drawSectionText(painter, sectionName, width, Qt::AlignCenter, topLeft, availableHeight);

            // Draw section elemnts
            for (auto &&e : elements) {
                // TODO: full signature should be displayed, not just name like now
                QString name(cutText(e->name(), painter->fontMetrics(), width));
                name.prepend(Util::sectionToSymbol(e->section()) + QChar::Space);
                if (!drawSectionText(painter, name, width, Qt::AlignLeft | Qt::AlignVCenter,
                                     topLeft, availableHeight))
                    break;
            }

            // Draw delimiter line
            if (!qFuzzyCompare(availableHeight, 0.)) {
                painter->setPen(frameColor);
                painter->drawLine(topLeft, QPointF(topLeft.x() + width, topLeft.y()));
            }

            painter->restore();
        }
    }

    /**
     * @brief Entity::Entity
     * @param parent
     */
    Entity::Entity(const entity::SharedType &type, QGraphicsItem *parent)
        : QGraphicsObject(parent)
        , m_Type(type)
        , m_LastPos(0, 0)
        , m_ResizeMode(false)
        , m_selectedToConnect(false)
    {
        setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsScenePositionChanges);

        if (width() < minimumWidth)
            setWidth(minimumWidth);

        if (height() < minimumHeight)
            setHeight(minimumHeight);

        setPos(dataPos());

        setAcceptHoverEvents(true);
        setCursor(defaultCursorShape);

        connect();
    }

    /**
     * @brief Entity::~Entity
     */
    Entity::~Entity()
    {
    }

    /**
     * @brief Entity::boundingRect
     * @return
     */
    QRectF Entity::boundingRect() const
    {
        return QRectF(-width() / 2 - margin, -height() / 2 - margin,
                      width() + margin, height() + margin);
    }

    /**
     * @brief Entity::paint
     * @param painter
     * @param option
     * @param widget
     */
    void Entity::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        drawFrame(painter);
        drawHeader(painter);
        drawSections(painter);
        drawResizeCorner(painter);

        if (selectedToConnect())
            drawConnectFrame(painter);
    }

    QVariant Entity::itemChange(GraphicsItemChange change, const QVariant &value)
    {
        if (change == ItemPositionChange && scene() )
            emit positionChanged(pos(), value.toPointF());

        return QGraphicsItem::itemChange(change, value);
    }

    /**
     * @brief Entity::type
     * @return
     */
    int Entity::type() const
    {
        return Type;
    }

    /**
     * @brief Entity::typeObject
     * @return
     */
    entity::SharedType Entity::typeObject() const
    {
        return m_Type;
    }

    /**
     * @brief Entity::setTypeObject
     * @param type
     */
    void Entity::setTypeObject(const entity::SharedType &type)
    {
        // Disconnect old type
        if (m_Type)
            connect(false);

        // Setup new one
        m_Type = type;
        connect(true);
        setPos(dataPos());

        // Update
        redraw();
    }

    /**
     * @brief Entity::id
     * @return
     */
    common::ID Entity::id() const
    {
        return m_Type ? m_Type->id() : common::ID::nullID();
    }

    /**
     * @brief Entity::setSelectedToConnect
     * @param status
     */
    void Entity::setSelectedToConnect(bool status)
    {
        m_selectedToConnect = status;
    }

    /**
     * @brief Entity::selectedToConnect
     * @return
     */
    bool Entity::selectedToConnect() const
    {
        return m_selectedToConnect;
    }

    /**
     * @brief Entity::rectMargin
     * @return
     */
    qreal Entity::rectMargin()
    {
        return margin;
    }

    /**
     * @brief Entity::frameRect
     * @return
     */
    QRectF Entity::frameRect() const
    {
        return QRectF(-width() / 2, -height() / 2, width(), height());
    }

    /**
     * @brief Entity::redraw
     */
    void Entity::redraw()
    {
        // TODO: add flags and update only needed elements, e.g. header
        update(boundingRect());
    }

    /**
     * @brief Entity::mousePressEvent
     * @param event
     */
    void Entity::mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        m_LastPos = pos();

        m_ResizeMode = resizeCorner().containsPoint(event->pos(), Qt::OddEvenFill);

        QGraphicsItem::mousePressEvent(event);
    }

    /**
     * @brief Entity::mouseReleaseEvent
     * @param event
     */
    void Entity::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
    {
        if (pos() != m_LastPos)
            emit moved(m_LastPos, pos());

        m_ResizeMode = false;

        QGraphicsItem::mouseReleaseEvent(event);
    }

    /**
     * @brief Entity::mouseMoveEvent
     * @param event
     */
    void Entity::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
    {
        if (m_ResizeMode) {
            QPointF pos = event->pos();
            QPointF lastPos = event->lastPos();

            qreal yLen = (QPointF(lastPos.x(), pos.y()) - lastPos).manhattanLength();
            int yFactor = pos.y() > lastPos.y() ? 2 : -2; // Why 2? See coordinate system.

            qreal xLen = (event->pos() - QPointF(lastPos.x(), pos.y())).manhattanLength();
            int xFactor = pos.x() > lastPos.x() ? 2 : -2; // Why 2? See coordinate system.

            prepareGeometryChange();

            auto tmpHeight = height() + yLen * yFactor;
            if (tmpHeight >= minimumHeight)
                setHeight(tmpHeight);

            auto tmpWidth = width() + xLen * xFactor;
            if (tmpWidth >= minimumWidth)
                setWidth(tmpWidth);

            emit sizeChanged();
        }
        else
            QGraphicsItem::mouseMoveEvent(event);
    }

    /**
     * @brief Entity::hoverMoveEvent
     * @param event
     */
    void Entity::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
    {
        if (resizeCorner().containsPoint(event->pos(), Qt::OddEvenFill) && !selectedToConnect())
            setCursor(Qt::SizeFDiagCursor);
        else
            setCursor(defaultCursorShape);

        QGraphicsItem::hoverMoveEvent(event);
    }

    /**
     * @brief Entity::resizeCornerSize
     * @return
     */
    QPolygonF Entity::resizeCorner() const
    {
        QRectF rect(resizeCornerRect());
        return QPolygonF() << rect.topRight() << rect.bottomLeft() << rect.bottomRight();
    }

    /**
     * @brief Entity::resizeCornerRect
     * @return
     */
    QRectF Entity::resizeCornerRect() const
    {
        return QRectF(width() / 2  - resizeCornerSize.width(),
                      height() / 2 - resizeCornerSize.height(),
                      resizeCornerSize.width(), resizeCornerSize.height());
    }

    /**
     * @brief Entity::width
     * @return
     */
    qreal Entity::width() const
    {
        return G_ASSERT(G_ASSERT(m_Type)->graphicEntityData())->width();
    }

    /**
     * @brief Entity::setWidth
     * @param newWidth
     */
    void Entity::setWidth(qreal newWidth)
    {
        G_ASSERT(G_ASSERT(m_Type)->graphicEntityData())->setWidth(newWidth);
    }

    /**
     * @brief Entity::height
     * @return
     */
    qreal Entity::height() const
    {
        return G_ASSERT(G_ASSERT(m_Type)->graphicEntityData())->height();
    }

    /**
     * @brief Entity::setHeight
     * @param newHeight
     */
    void Entity::setHeight(qreal newHeight)
    {
        G_ASSERT(G_ASSERT(m_Type)->graphicEntityData())->setHeight(newHeight);
    }

    /**
     * @brief Entity::dataPos
     * @return
     */
    QPointF Entity::dataPos() const
    {
        return G_ASSERT(G_ASSERT(m_Type)->graphicEntityData())->pos();
    }

    /**
     * @brief Entity::color
     * @return
     */
    QColor Entity::typeColor() const
    {
        return App::Settings::elementColor(
                    entity::kindOfTypeToString(G_ASSERT(m_Type)->kindOfType()));
    }

    void Entity::connect(bool connect)
    {
        entity::GraphicEntityData *data = G_ASSERT(m_Type->graphicEntityData()).get();

        if (connect) {
            G_CONNECT(G_ASSERT(m_Type.get()), &common::BasicElement::nameChanged,
                      this, &Entity::redraw);
            G_CONNECT(this, &Entity::positionChanged,
                      G_ASSERT(data), &entity::GraphicEntityData::onPosChanged);
        } else {
            G_DISCONNECT(G_ASSERT(m_Type.get()), &common::BasicElement::nameChanged,
                         this, &Entity::redraw);
            G_DISCONNECT(this, &Entity::positionChanged,
                         G_ASSERT(data), &entity::GraphicEntityData::onPosChanged);
        }
    }

    /**
     * @brief Entity::drawHeader
     * @param painter
     */
    void Entity::drawHeader(QPainter *painter)
    {
        painter->save();

        QColor color = typeColor();

        // Fill background
        QLinearGradient gradient(0, -height() / 2, 0, -height() / 2 + minimumHeight );
        gradient.setColorAt(0, color);
        gradient.setColorAt(1, Qt::white);
        QRectF headerRect(-width() / 2, -height() / 2, width(), minimumHeight);
        painter->fillRect(headerRect, QBrush(gradient));

        // Draw frame
        painter->setPen(color);
        painter->drawRect(headerRect);

        // Add element name
        painter->setPen(Qt::black);
        painter->setRenderHint(QPainter::TextAntialiasing);
        QString name(cutText(G_ASSERT(m_Type)->name(), painter->fontMetrics(), width()));
        painter->drawText(headerRect, Qt::AlignCenter, name);

        painter->restore();
    }

    /**
     * @brief Entity::drawFrame
     * @param painter
     */
    void Entity::drawFrame(QPainter *painter)
    {
        painter->save();

        QColor color = typeColor();
        QRectF rect(frameRect());
        painter->fillRect(rect, Qt::white);

        painter->setPen(color);
        painter->drawRect(rect);

        painter->restore();
    }

    /**
     * @brief Entity::drawResizeCorner
     * @param painter
     */
    void Entity::drawResizeCorner(QPainter *painter)
    {
        painter->save();

        painter->setRenderHint(QPainter::Antialiasing);

        painter->setPen(typeColor());

        QRectF rect(resizeCornerRect());
        QPointF bottomLeft(rect.bottomLeft());
        QPointF topRight(rect.topRight());

        // Draw hypotenuse
        painter->drawLine(bottomLeft, topRight);

        // Draw middle line
        bottomLeft.rx() += rect.width() / 2;
        topRight.ry() += rect.height() / 2;
        painter->drawLine(bottomLeft, topRight);

        painter->restore();
    }

    /**
     * @brief Entity::drawSections
     * @param painter
     */
    void Entity::drawSections(QPainter *painter)
    {
        painter->save();

        // Calculate initial parameters
        QColor color = typeColor();
        auto topLeft = boundingRect().topLeft() + QPointF(margin, margin);
        topLeft.ry() += minimumHeight;
        qreal len = height() - minimumHeight;

        // Draw sections
        drawSection(painter, tr("Properties"), m_Type->properties(), topLeft, len, width(), color);
        drawSection(painter, tr("Methods"), m_Type->methods(), topLeft, len, width(), color);
        drawSection(painter, tr("Fields"), m_Type->fields(), topLeft, len, width(), color);
        drawSection(painter, tr("Elements"), m_Type->enumerators(), topLeft, len, width(), color);

        painter->restore();
    }

    /**
     * @brief Entity::drawConnectFrame
     * @param painter
     */
    void Entity::drawConnectFrame(QPainter *painter)
    {
        painter->save();

        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(QPen(Qt::red, 2));
        painter->drawRect(frameRect());

        painter->restore();
    }

} // grpahics
