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
#include<QDebug>

#include <application/settings.h>

#include <gui/editentitydialog.h>

#include <entity/type.h>
#include <entity/enum.h>
#include <entity/union.h>
#include <entity/class.h>
#include <entity/templateclass.h>
#include <entity/extendedtype.h>
#include <entity/scope.h>

#include "constants.h"
#include "qthelpers.h"

namespace graphics {

    namespace {
        constexpr qreal margin    = 2. ;
        constexpr qreal minimumHeight = 20.;
        constexpr qreal minimumWidth  = 120.;
        constexpr qreal paddingPercent = 0.1;
        const QString stub = Entity::tr( "Stub" );

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
        , m_Width(minimumWidth)
        , m_Height(minimumHeight)
        , m_HeaderHeight(minimumHeight)
        , m_Scope(scope)
        , m_Project(project)
    {
        setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

        Q_ASSERT(type);
        connect(type.get(), &entity::BasicEntity::nameChanged, [=]{ update(); });
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

        drawHeader(painter);
        drawFrame(painter);
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
     * @brief Entity::event
     * @param ev
     * @return
     */
    bool Entity::sceneEvent(QEvent *ev)
    {
        switch (ev->type()) {
            case QEvent::GraphicsSceneMousePress:
            {
                m_LastPos = pos();
                break;
            }

            case QEvent::GraphicsSceneMouseRelease:
            {
                if (pos() != m_LastPos)
                    emit moved(m_LastPos, pos());

                break;
            }

            default: ;
        }

        return QGraphicsObject::sceneEvent(ev);
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
        QLinearGradient gradient(0, -m_Height / 2, 0, m_HeaderHeight / 2);
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
        painter->setPen(color);
        painter->drawRect(QRectF(-m_Width / 2, -m_Height / 2, m_Width, m_Height));

        painter->restore();
    }

} // grpahics
