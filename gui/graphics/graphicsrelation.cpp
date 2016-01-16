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

        const QPointF defaultPoint(std::numeric_limits<qreal>::max(), std::numeric_limits<qreal>::max());

        /// Find first intersection point of polyline segment and line
        inline QPointF intersection(const QPolygonF &p, const QLineF &l)
        {
            Q_ASSERT(p.count() == 4);

            QPointF result(defaultPoint);
            for (int i = 0, j = 1, c = p.count(); j < c; ++i, ++j)
                if (QLineF(p[i], p[j]).intersect(l, &result) == QLineF::BoundedIntersection)
                    break;

            Q_ASSERT(result != defaultPoint);
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
        initEntity(m_From);
        initEntity(m_To);
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
        setEntity(m_From, from);
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
        setEntity(m_To, to);
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
     * @brief Relation::initEntity
     * @param e
     */
    void Relation::initEntity(Entity *e)
    {
        if (e) {
            recalculateLine();
            G_CONNECT(e, &Entity::positionChanged, this, &Relation::recalculateLine);
            G_CONNECT(e, &Entity::sizeChanged, this, &Relation::recalculateLine);
        }
    }

    void Relation::setEntity(Entity *e, Entity *newEntity)
    {
        if (e) {
            G_DISCONNECT(e, &Entity::positionChanged, this, &Relation::recalculateLine);
            G_DISCONNECT(e, &Entity::sizeChanged, this, &Relation::recalculateLine);
        }

        e = newEntity;
        initEntity(e);
    }

    /**
     * @brief Relation::recalculateLine
     */
    void Relation::recalculateLine()
    {
        QPolygonF fromRect(m_From->mapToScene(m_From->frameRect()));
        QPolygonF toRect(m_To->mapToScene(m_To->frameRect()));
        setVisible(fromRect.intersected(toRect).isEmpty());

        if (isVisible()) {
            setP1(intersection(fromRect, QLineF(m_From->pos(), line().p2())));
            setP2(intersection(toRect, QLineF(line().p1(), m_To->pos())));
        }
    }

} // namespace graphics

