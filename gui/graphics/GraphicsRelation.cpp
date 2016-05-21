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

        const QPointF defaultPoint(std::numeric_limits<qreal>::max(),
                                   std::numeric_limits<qreal>::max());
        const qreal penWidth = 1.2f;
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

        QPointF heightPoint(const QLineF &line, qreal multFactor = 1.)
        {
           QLineF tmp(line);
           tmp.setLength(line.length() - arrowHeight * multFactor);
           return tmp.p2();
        }

        QLineF normal(const QLineF &line, const QPointF &p, qreal len, bool inverted = false)
        {
           qreal percent = len / line.length();
           qreal dy = line.dy() * percent;
           qreal dx = line.dx() * percent;
           return QLineF(p, p + QPointF(inverted ? -dy : dy, inverted ? dx : -dx));
        }

        QVector<QPointF> arrowPoints(const QLineF &l)
        {
            QPointF hp = heightPoint(l);
            QPointF left = normal(l, hp, arrowNormal).p2();
            QPointF right = normal(l, hp, arrowNormal, true /*inverted*/).p2();

            return {left, right};
        }

        void drawArrow(QPainter *p, const QLineF &l, const QPointF &startPoint)
        {
          p->save();

          auto ap = arrowPoints(l);
          p->drawLine(startPoint, ap[0]);
          p->drawLine(startPoint, ap[1]);

          p->restore();
        }

        void drawLineWithArrow(QPainter *p, const QLineF &l, bool dashLine = false)
        {
            p->save();

            drawArrow(p, l, l.p2());

            if (dashLine)
                p->setPen(Qt::DashLine);

            p->drawLine(l);

            p->restore();
        }

        void drawTriangleLine(QPainter *p, const QLineF &l)
        {
            p->save();

            p->drawLine(l);

            p->setBrush(Qt::white);
            auto ap = arrowPoints(l);
            p->drawPolygon(QVector<QPointF>{l.p2(), ap[0], ap[1]});

            p->restore();
        }

        void drawLineWithRect(QPainter * p, const QLineF &l, bool black = false)
        {
            p->save();

            p->drawLine(l);

            QVector<QPointF> points;
            points << l.p2();

            auto ap = arrowPoints(l);
            points << ap[0];
            points << heightPoint(l, 2.);
            points << ap[1];

            p->setBrush(black ? Qt::black : Qt::white);
            p->drawPolygon(points);

            p->restore();
        }

        using DrawingFunctor = std::function<void(QPainter *, const QLineF &)>;
        const QHash<relationship::RelationType, DrawingFunctor> drawingMap = {
            { relationship::AssociationRelation,
              [](QPainter *p, const QLineF &l){ drawLineWithArrow(p, l); } },
            { relationship::DependencyRelation,
              [](QPainter *p, const QLineF &l){ drawLineWithArrow(p, l, true /*dashLine*/); } },
            { relationship::GeneralizationRelation,
              [](QPainter *p, const QLineF &l){ drawTriangleLine(p, l); } },
            { relationship::CompositionRelation,
              [](QPainter *p, const QLineF &l){ drawLineWithRect(p, l, true /*black*/); } },
            { relationship::AggregationRelation,
              [](QPainter *p, const QLineF &l){ drawLineWithRect(p, l); } },
        };
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

        // Set pen parameters
        QPen pen = painter->pen();
        pen.setWidthF(penWidth);
        painter->setPen(pen);

        // Draw relation
        auto it = drawingMap.find(m_Relation->relationType());
        if (it != drawingMap.end())
            (*it)(painter, line());

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

