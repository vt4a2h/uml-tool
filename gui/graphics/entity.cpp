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

namespace graphics {

    namespace {
        constexpr double margin = 2.;
        constexpr double tmpHeight = 100.;
        constexpr double tmpWidth = 100.;
    }

    /**
     * @brief Entity::Entity
     * @param parent
     */
    Entity::Entity(QGraphicsItem *parent)
        : QGraphicsObject(parent)
    {
        setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
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
    }

} // grpahics
