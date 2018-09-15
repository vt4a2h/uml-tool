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
#include "GraphicEntityData.h"

#include <QRectF>

#include <Utility/helpfunctions.h>

namespace Entity {

    namespace {
        const QString heightMark = "Height";
        const QString widthMark  = "Width";
        const QString xMark      = "X";
        const QString yMark      = "Y";
    }

    /**
     * @brief EntityData::EntityData
     */
    GraphicEntityData::GraphicEntityData()
        : m_Pos(0, 0)
        , m_Width(0)
        , m_Height(0)
    {
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const GraphicEntityData &lhs, const GraphicEntityData &rhs)
    {
        return lhs.m_Pos    == rhs.m_Pos   &&
               lhs.m_Width  == rhs.m_Width &&
               lhs.m_Height == rhs.m_Height;
    }

    /**
     * @brief EntityData::toJson
     * @return
     */
    QJsonObject GraphicEntityData::toJson() const
    {
        QJsonObject result;

        result[heightMark] = m_Height;
        result[widthMark]  = m_Width;
        result[xMark]      = m_Pos.x();
        result[yMark]      = m_Pos.y();

        return result;
    }

    /**
     * @brief EntityData::fromJson
     * @param src
     * @param errorList
     */
    void GraphicEntityData::fromJson(const QJsonObject &src, ErrorList &errorList)
    {
        Util::checkAndSet(src, heightMark, errorList, [&src, &errorList, this](){
            m_Height = src[heightMark].toDouble();
        });
        Util::checkAndSet(src, widthMark, errorList, [&src, &errorList, this](){
            m_Width = src[widthMark].toDouble();
        });

        QPointF pos(0., 0.);
        Util::checkAndSet(src, xMark, errorList, [&src, &pos](){
            pos.rx() = src[xMark].toDouble();
        });
        Util::checkAndSet(src, yMark, errorList, [&src, &pos](){
            pos.ry() = src[yMark].toDouble();
        });
        setPos(pos);
    }

    /**
     * @brief EntityData::pos
     * @return
     */
    QPointF GraphicEntityData::pos() const
    {
        return m_Pos;
    }

    /**
     * @brief EntityData::setPos
     * @param pos
     */
    void GraphicEntityData::setPos(const QPointF &pos)
    {
        m_Pos = pos;
    }

    /**
     * @brief EntityData::width
     * @return
     */
    qreal GraphicEntityData::width() const
    {
        return m_Width;
    }

    /**
     * @brief EntityData::setWidth
     * @param width
     */
    void GraphicEntityData::setWidth(qreal width)
    {
        m_Width = width;
    }

    /**
     * @brief EntityData::height
     * @return
     */
    qreal GraphicEntityData::height() const
    {
        return m_Height;
    }

    /**
     * @brief EntityData::setHeight
     * @param height
     */
    void GraphicEntityData::setHeight(qreal height)
    {
        m_Height = height;
    }

    /**
     * @brief GraphicEntityData::boundingRect
     * @return
     */
    QRectF GraphicEntityData::boundingRect() const
    {
        QRectF rect;
        rect.setWidth(width());
        rect.setHeight(height());
        rect.moveCenter(pos());

        return rect;
    }

    /**
     * @brief EntityData::onPosChanged
     * @param from
     * @param to
     */
    void GraphicEntityData::onPosChanged(const QPointF &from, const QPointF &to)
    {
        Q_UNUSED(from)
        m_Pos = to;
    }

} // namespace graphics
