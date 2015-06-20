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

#include <gui/editentitydialog.h>

#include <entity/type.h>
#include <entity/scope.h>

#include <constants.h>

namespace graphics {

    namespace {
        constexpr double margin    = 2.  ;
        constexpr double tmpHeight = 100.;
        constexpr double tmpWidth  = 100.;
        const QString stub = Entity::tr( "Stub" );
    }

    /**
     * @brief Entity::Entity
     * @param parent
     */
    Entity::Entity(const entity::SharedType &type, const entity::SharedScope &scope,
                   const project::SharedProject &project, QGraphicsItem *parent)
        : QGraphicsObject(parent)
        , m_Type(type)
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
        return QRectF(-tmpWidth / 2 - margin, -tmpHeight / 2 - margin,
                      tmpWidth + margin, tmpHeight + margin);
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
        painter->setBrush(Qt::black);
        painter->drawRect(QRectF(-tmpWidth / 2, -tmpHeight / 2, tmpWidth, tmpHeight));
        painter->setPen(Qt::white);
        painter->drawText(boundingRect(), Qt::AlignCenter, m_Type ? m_Type->name() : stub);
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

} // grpahics
