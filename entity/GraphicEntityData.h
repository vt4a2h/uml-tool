/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 05/05/2016.
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

#include <QPointF>
#include <QJsonObject>
#include <QObject>

#include <types.h>

namespace entity {

    /// Data for graphical representation of entities
    class GraphicEntityData : public QObject
    {
        Q_OBJECT

    public:
        GraphicEntityData();
        GraphicEntityData(GraphicEntityData &&) noexcept = default;
        GraphicEntityData(const GraphicEntityData &) = default;

        GraphicEntityData& operator= (GraphicEntityData &&) noexcept = default;
        GraphicEntityData& operator= (const GraphicEntityData &) = default;

        QJsonObject toJson() const;
        void fromJson(const QJsonObject &src, ErrorList &errorList);

        QPointF pos() const;
        void setPos(const QPointF &pos);

        qreal width() const;
        void setWidth(const qreal &width);

        qreal height() const;
        void setHeight(const qreal &height);

    public slots:
        void onPosChanged(const QPointF &from, const QPointF &to);
        void onHeightChanged(qreal height);
        void onWidthChanged(qreal width);

    private:
        QPointF m_Pos;
        qreal m_Width;
        qreal m_Height;
    };

} // namespace graphics
