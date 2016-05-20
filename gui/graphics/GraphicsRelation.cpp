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
#include "GraphicsRelation.h"

#include <math.h>

#include <QPainter>

#include <relationship/Relation.h>

#include "Entity.h"
#include "qthelpers.h"
#include "enums.h"

namespace graphics {

    namespace  {

        const QPointF defaultPoint(std::numeric_limits<qreal>::max(), std::numeric_limits<qreal>::max());
        const qreal arrowHeight = 10.;
        const qreal arrowNormal = tan(30 * M_PI / 180) * arrowHeight;

        /// Find first intersection point of polyline segment and line
        QPointF intersection(const QPolygonF &p, const QLineF &l)
        {
//            Q_ASSERT(p.count() == 4);

            QPointF result(defaultPoint);
            for (int i = 0, j = 1, c = p.count(); j < c; ++i, ++j)
                if (QLineF(p[i], p[j]).intersect(l, &result) == QLineF::BoundedIntersection)
                    break;

            Q_ASSERT(result != defaultPoint);
            return result;
        }

        QPointF heightPoint(const QLineF &line)
        {
           QLineF tmp(line);
           tmp.setLength(line.length() - arrowHeight);
           return tmp.p2();
        }

        QLineF normal(const QLineF &line, const QPointF &p, qreal len, bool inverted = false)
        {
           qreal percent = len / line.length();
           qreal dy = line.dy() * percent;
           qreal dx = line.dx() * percent;
           return QLineF(p, p + QPointF(inverted ? -dy : dy, inverted ? dx : -dx));
        }

        void drawArrow(QPainter *p, const QLineF &l, const QPointF &startPoint)
        {
          p->save();

          QPointF hp = heightPoint(l);
          p->drawLine(startPoint, normal(l, hp, arrowNormal).p2());
          p->drawLine(startPoint, normal(l, hp, arrowNormal, true /*inverted*/).p2());

          p->restore();
        }
    }

    /**
     * @brief Relation::Relation
     * @param relation
     * @param from
     * @param to
     * @param parent
     */
    Relation::Relation(const relationship::SharedRelation &relation, const EntityPtr &from,
                       const EntityPtr &to, QGraphicsItem *parent)
        : QGraphicsLineItem(parent)
        , m_Relation(relation)
        , m_From(from)
        , m_To(to)
    {
        initEntity(m_From);
        initEntity(m_To);
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
    void Relation::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);

        QPen pen = painter->pen();
        pen.setWidthF(1.5f);
        painter->setPen(pen);
        painter->drawLine(line());

        if (m_Relation->relationType() == relationship::DependencyRelation ||
            m_Relation->relationType() == relationship::AssociationRelation)
            drawArrow(painter, line(), line().p2());

        painter->restore();
    }

    /**
     * @brief Relation::boundingRect
     * @return
     */
    QRectF Relation::boundingRect() const
    {
      auto result = QGraphicsLineItem::boundingRect();
//      result.setWidth(result.width() + 2 * arrowHeight);
//      result.setHeight(result.height() + 2 * arrowHeight);
//      result.setLeft(result.bottom() + arrowHeight);
//      result.setLeft(result.left() + arrowHeight);
      return result;
    }

    /**
     * @brief Relation::initEntity
     * @param e
     */
    void Relation::initEntity(const EntityPtr &e)
    {
        if (e) {
            recalculateLine();
            G_CONNECT(e.data(), &Entity::positionChanged, this, &Relation::recalculateLine);
            G_CONNECT(e.data(), &Entity::sizeChanged, this, &Relation::recalculateLine);
        }
    }

    /**
     * @brief Relation::setEntity
     * @param e
     * @param newEntity
     */
    void Relation::setEntity(EntityPtr &e, const EntityPtr &newEntity)
    {
        if (e) {
            G_DISCONNECT(e.data(), &Entity::positionChanged, this, &Relation::recalculateLine);
            G_DISCONNECT(e.data(), &Entity::sizeChanged, this, &Relation::recalculateLine);
        }

        e = newEntity;
        initEntity(e);
    }

    /**
     * @brief Relation::to
     * @return
     */
    EntityPtr Relation::to() const
    {
       return m_To;
    }

    /**
     * @brief Relation::setTo
     * @param to
     */
    void Relation::setTo(const EntityPtr &to)
    {
        setEntity(m_To, to);
    }

    /**
     * @brief Relation::id
     * @return
     */
    common::ID Relation::id() const
    {
        return m_Relation ? m_Relation->id() : common::ID::nullID();
    }

    /**
     * @brief Relation::setFrom
     * @param from
     */
    void Relation::setFrom(const EntityPtr &from)
    {
        setEntity(m_From, from);
    }

    /**
     * @brief Relation::From
     * @return
     */
    EntityPtr Relation::from() const
    {
        return m_From;
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

            auto l = line();
            l.setLength(l.length() - 2);
            setLine(l);
        }
    }

} // namespace graphics

