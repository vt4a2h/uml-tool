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
#include "entity.h"

#include <QPainter>
#include <QEvent>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsScene>
#include <QDebug>

#include <application/settings.h>

#include <gui/editentitydialog.h>

#include <entity/type.h>
#include <entity/enum.h>
#include <entity/union.h>
#include <entity/class.h>
#include <entity/templateclass.h>
#include <entity/extendedtype.h>
#include <entity/scope.h>

#include <entity/property.h>

#include "constants.h"
#include "qthelpers.h"

namespace graphics {

    namespace {
        constexpr qreal margin    = 2. ;
        constexpr qreal minimumHeight = 30.;
        constexpr qreal lineHeight    = 20.;
        constexpr qreal minimumWidth  = 120.;
        constexpr qreal paddingPercent = 0.1;
        const QSizeF resizeCornerSize(12, 12);
        const Qt::CursorShape defaultCursorShape = Qt::ArrowCursor;

        /// Cuts too long text
        QString cutText(const QString &text, const QFontMetrics &fm, qreal frameWidth)
        {
            QString result(text);
            QSize textSize(fm.size(Qt::TextSingleLine, text));
            int maxTextWidth = frameWidth * (1 - paddingPercent);
            if (textSize.width() > maxTextWidth) {
                int symbolsCount = maxTextWidth / (textSize.width() / result.count());
                symbolsCount -= 3; // three dots
                result.chop(result.count() - symbolsCount);
                result.append("...");
            }

            return result;
        }

        template <class Container>
        QPointF drawSection(QPainter * painter, const QString &sectionName, const Container elements,
                            const QPointF &topLeft, qreal availableHeight, qreal width,
                            const QColor &frameColor)
        {
            painter->save();

            QPointF bottomLeft = topLeft;
            bottomLeft.ry() += lineHeight;
            painter->drawText(QRectF(topLeft + QPointF(2 * margin, 0), QSizeF(width, lineHeight)), Qt::AlignLeft | Qt::AlignVCenter, sectionName);

            for (auto &&e : elements) {
                qDebug() << "diff:" << (bottomLeft - topLeft).manhattanLength();
                qDebug() << (bottomLeft - topLeft).manhattanLength() << ">" << availableHeight << "=" << ((bottomLeft - topLeft).manhattanLength() > availableHeight);
                if ((bottomLeft - topLeft).manhattanLength() > availableHeight) {
                    qDebug() << "broken";
                    break;
                }

                // TODO: deal with it
                painter->drawText(QRectF(bottomLeft + QPointF(2 * margin, 0), QSizeF(width, lineHeight)), Qt::AlignLeft | Qt::AlignVCenter, e->name());
                bottomLeft.ry() += lineHeight;
            }

            qreal height = lineHeight * (elements.count() + 1 /*section name*/);
            qDebug() << "height:" << height;
            if (height < availableHeight ) {
                QRectF rect(topLeft, QSizeF(width, height));
                painter->setPen(frameColor);
                painter->drawRect(rect);
            }

            painter->restore();
        }
    }

    /**
     * @brief Entity::Entity
     * @param parent
     */
    Entity::Entity(const entity::SharedType &type, const entity::SharedScope &scope,
                   const project::SharedProject &project, QGraphicsItem *parent)
        : QGraphicsObject(parent)
        , m_Type(type)
        , m_LastPos(0, 0)
        , m_ResizeMode(false)
        , m_Width(minimumWidth)
        , m_Height(/*minimumHeight*/200)
        , m_HeaderHeight(minimumHeight)
        , m_Scope(scope)
        , m_Project(project)
    {
        setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

        setAcceptHoverEvents(true);
        setCursor(defaultCursorShape);

        connect(G_ASSERT(type.get()), &entity::BasicEntity::nameChanged, [=]{ update(); });

        // TODO: remove. Just for test
        m_Type->addNewProperty()->setName("p1");
        m_Type->addNewProperty()->setName("p2");
        m_Type->addNewProperty()->setName("p3");
        m_Type->addNewProperty()->setName("p4");
        m_Type->addNewProperty()->setName("p5");
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
        return QRectF(-m_Width / 2 - margin, -m_Height / 2 - margin,
                      m_Width + margin, m_Height + margin);
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
        drawResizeCorner(painter);

        // TODO: move to the separate method {
        QColor color = application::settings::elementColor(G_ASSERT(m_Type)->marker());

        auto topLeft = boundingRect().topLeft() + QPointF(margin, margin);
        topLeft.ry() += m_HeaderHeight;
        qreal len = m_Height - m_HeaderHeight;
        qDebug() << "len:" << len
                 << "topLeft:" << topLeft;
        topLeft = drawSection(painter, tr("Properties"), m_Type->properties(), topLeft, len,
                              m_Width, color);
        // }
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
        m_Type = type;
    }

    /**
     * @brief Entity::scope
     * @return
     */
    entity::SharedScope Entity::scope() const
    {
        return m_Scope;
    }

    /**
     * @brief Entity::setScope
     * @param scope
     */
    void Entity::setScope(const entity::SharedScope &scope)
    {
        m_Scope = scope;
    }

    /**
     * @brief Entity::project
     * @return
     */
    project::SharedProject Entity::project() const
    {
        return m_Project;
    }

    /**
     * @brief Entity::setProject
     * @param project
     */
    void Entity::setProject(const project::SharedProject &project)
    {
        m_Project = project;
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

            auto tmpHeight = m_Height + yLen * yFactor;
            if (tmpHeight >= minimumHeight)
                m_Height = tmpHeight;

            auto tmpWidth = m_Width + xLen * xFactor;
            if (tmpWidth >= minimumWidth)
                m_Width = tmpWidth;
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
        if (resizeCorner().containsPoint(event->pos(), Qt::OddEvenFill))
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
        return QRectF(m_Width / 2  - resizeCornerSize.width(),
                      m_Height / 2 - resizeCornerSize.height(),
                      resizeCornerSize.width(), resizeCornerSize.height());
    }

    /**
     * @brief Entity::drawHeader
     * @param painter
     */
    void Entity::drawHeader(QPainter *painter)
    {
        painter->save();

        QColor color = application::settings::elementColor(G_ASSERT(m_Type)->marker());

        // Fill background
        QLinearGradient gradient(0, -m_Height / 2, 0, -m_Height / 2 + m_HeaderHeight);
        gradient.setColorAt(0, color);
        gradient.setColorAt(1, Qt::white);
        QRectF headerRect(-m_Width / 2, -m_Height / 2, m_Width, m_HeaderHeight);
        painter->fillRect(headerRect, QBrush(gradient));

        // Draw frame
        painter->setPen(color);
        painter->drawRect(headerRect);

        // Add element name
        painter->setPen(Qt::black);
        painter->setRenderHint(QPainter::TextAntialiasing);
        QString name(cutText(G_ASSERT(m_Type)->name(), painter->fontMetrics(), m_Width));
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

        QColor color = application::settings::elementColor(G_ASSERT(m_Type)->marker());
        QRectF rect(-m_Width / 2, -m_Height / 2, m_Width, m_Height);
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

        painter->setPen(application::settings::elementColor(G_ASSERT(m_Type)->marker()));
        QRectF rect(resizeCornerRect());

        QPointF bottomLeft(rect.bottomLeft());
        QPointF topRight(rect.topRight());
        painter->drawLine(bottomLeft, topRight);

        bottomLeft.rx() += rect.width() / 2;
        topRight.ry() += rect.height() / 2;
        painter->drawLine(bottomLeft, topRight);

        painter->restore();
    }

} // grpahics
