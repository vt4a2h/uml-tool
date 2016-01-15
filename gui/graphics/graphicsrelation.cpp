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

#include <QPainter>

#include <relationship/relation.h>

#include "entity.h"
#include "qthelpers.h"

namespace graphics {

    namespace  {

        /// Find first intersection point of polyline segment and line
        inline QPointF intersection(const QPolygonF &p, const QLineF &l)
        {
            Q_ASSERT(p.count() == 4);

            QPointF result;
            for (int i = 0, j = 1, c = p.count(); j < c; ++i, ++j)
                if (QLineF(p[i], p[j]).intersect(l, &result) == QLineF::BoundedIntersection)
                    break;

            return result;
        }

    }

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
            G_DISCONNECT(m_From, &Entity::positionChanged, this, &Relation::recalculateLine);

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
            G_DISCONNECT(m_To, &Entity::positionChanged, this, &Relation::recalculateLine);

        m_To = to;
        initTo();
    }

    /**
     * @brief Relation::setP1
     * @param p
     */
    void Relation::setP1(const QPointF &p)
    {
        setLine(QLineF(p, line().p2()));
    }

    /**
     * @brief Relation::setP2
     * @param p
     */
    void Relation::setP2(const QPointF &p)
    {
        setLine(QLineF(line().p1(), p));
    }

    /**
     * @brief Relation::paint
     * @param painter
     * @param option
     * @param widget
     */
    void Relation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        painter->setRenderHint(QPainter::Antialiasing);
        QGraphicsLineItem::paint(painter, option, widget);
    }

    /**
     * @brief Relation::initFrom
     */
    void Relation::initFrom()
    {
        if (m_From) {
            recalculateLine();
            G_CONNECT(m_From, &Entity::positionChanged, this, &Relation::recalculateLine);
        }
    }

    /**
     * @brief Relation::initTo
     */
    void Relation::initTo()
    {
        if (m_To) {
            recalculateLine();
            G_CONNECT(m_To, &Entity::positionChanged, this, &Relation::recalculateLine);
        }
    }

    /**
     * @brief Relation::recalculateLine
     */
    void Relation::recalculateLine()
    {
        // TODO: add null checkings for intersection point
        setP1(intersection(m_From->mapToScene(m_From->frameRect()), QLineF(m_From->pos(), line().p2())));
        setP2(intersection(m_To->mapToScene(m_To->frameRect()), QLineF(line().p1(), m_To->pos())));
    }

} // namespace graphics

