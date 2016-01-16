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
#pragma once

#include <QGraphicsLineItem>

#include <relationship/relationship_types.hpp>

namespace graphics {

    class Entity;

    /// Graphics represenation of relation
    class Relation : public QObject, public QGraphicsLineItem
    {
        Q_OBJECT

    public:
        Relation(const relationship::SharedRelation &relation, Entity *from, Entity *to,
                 QGraphicsItem *parent = nullptr);

        Entity *from() const;
        void setFrom(Entity *from);

        Entity *to() const;
        void setTo(Entity *to);

        void setP1(const QPointF &p);
        void setP2(const QPointF &p);

    public: // QGraphicsLineItem overrides
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    private slots:
        void recalculateLine();

    private:
        void initEntity(Entity * e);
        void setEntity(Entity * e, Entity * newEntity);

        relationship::SharedRelation m_Relation;
        Entity * m_From;
        Entity * m_To;
    };

} // namespace graphics
