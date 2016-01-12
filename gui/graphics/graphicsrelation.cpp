/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 10/01/2016.
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
#include "graphicsrelation.h"

#include <relationship/relation.h>

#include "entity.h"
#include "qthelpers.h"

namespace graphics {

    /**
     * @brief Relation::Relation
     * @param relation
     * @param from
     * @param to
     * @param parent
     */
    Relation::Relation(const relationship::SharedRelation &relation, Entity *from, Entity *to,
                       QGraphicsItem *parent)
        : QGraphicsLineItem(parent)
        , m_Relation(relation)
        , m_From(from)
        , m_To(to)
    {
        initFrom();
        initTo();
    }

    /**
     * @brief Relation::from
     * @return
     */
    Entity *Relation::from() const
    {
        return m_From;
    }

    /**
     * @brief Relation::setFrom
     * @param from
     */
    void Relation::setFrom(Entity *from)
    {
        if (m_From)
            G_DISCONNECT(m_From, &Entity::positionChanged, this, &Relation::onFromChanged);

        m_From = from;
        initFrom();
    }

    /**
     * @brief Relation::to
     * @return
     */
    Entity *Relation::to() const
    {
        return m_To;
    }

    /**
     * @brief Relation::setTo
     * @param to
     */
    void Relation::setTo(Entity *to)
    {
        if (m_To)
            G_DISCONNECT(m_To, &Entity::positionChanged, this, &Relation::onToChanged);

        m_To = to;
        initTo();
    }

    /**
     * @brief Relation::onFromChanged
     * @param oldPos
     * @param newPos
     */
    void Relation::onFromChanged(const QPointF &, const QPointF &newPos)
    {
        setLine(QLineF(newPos, line().p2()));
    }

    void Relation::onToChanged(const QPointF &, const QPointF &newPos)
    {
        setLine(QLineF(line().p1(), newPos));
    }

    /**
     * @brief Relation::initFrom
     */
    void Relation::initFrom()
    {
        if (m_To) {
            setLine(QLineF(m_From->pos(), line().p2()));
            G_CONNECT(m_From, &Entity::positionChanged, this, &Relation::onFromChanged);
        }
    }

    /**
     * @brief Relation::initTo
     */
    void Relation::initTo()
    {
        if (m_To) {
            setLine(QLineF(line().p1(), m_To->pos()));
            G_CONNECT(m_To, &Entity::positionChanged, this, &Relation::onToChanged);
        }
    }

} // namespace graphics

