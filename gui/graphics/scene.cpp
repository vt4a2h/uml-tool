/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 09/01/2016.
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
#include "scene.h"

#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QDebug>

#include "entity.h"
#include "qthelpers.h"

namespace graphics {

    namespace
    {

        /// Dash line with antialiasing
        class Line : public QGraphicsLineItem
        {
        public:
            void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget = nullptr)
            {
                painter->setRenderHint(QPainter::Antialiasing);
                setPen(Qt::DashLine);

                QGraphicsLineItem::paint(painter, option, widget);
            }

            enum { Type = UserType + int(ElementType::Relation) };
            int type() const { return Type; }
        };

        inline Entity * entElem(const Scene * scene, const QPointF &pos)
        {
            if (auto item = scene->itemAt(pos, QTransform()))
                return qgraphicsitem_cast<Entity*>(item);

            return nullptr;
        }

        inline void setTrackedItemStatus(QPointer<Entity> &e, bool status, bool update = true)
        {
            if (!e.isNull()) {
                e->setSelectedToConnect(status);

                if (update)
                    e->update();
            }
        }
    }

    /**
     * @brief Scene::Scene
     * @param parent
     */
    Scene::Scene(QObject *parent)
        : QGraphicsScene(parent)
        , m_ShowRelationTrack(false)
        , m_TrackRelationIsActive(false)
        , m_TrackFrom(nullptr)
        , m_TrackTo(nullptr)
    {
        initTrackLine();
    }

    /**
     * @brief Scene::~Scene
     */
    Scene::~Scene()
    {
    }

    /**
     * @brief Create new tracking line object. Usefull if scene was cleared.
     */
    void Scene::initTrackLine()
    {
        m_RelationTrackLine = new Line;
        addItem(m_RelationTrackLine); // transfer ownership
        m_RelationTrackLine->hide();
    }

    /**
     * @brief Scene::showRelationTrack
     * @return
     */
    bool Scene::showRelationTrack() const
    {
        return m_ShowRelationTrack;
    }

    /**
     * @brief Scene::mousePressEvent
     * @param event
     */
    void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        if (showRelationTrack()) {

            QPointF scenePos = event->scenePos();
            if (auto elem = entElem(this, scenePos)) {

                // Preserve and mark as selected first element
                m_TrackFrom = elem;
                setTrackedItemStatus(m_TrackFrom, true /*status*/, true/*update*/);

                // Display tracking line
                m_RelationTrackLine->setLine(QLineF(scenePos, scenePos));
                m_RelationTrackLine->show();

                // Set tracking status and accept event
                m_TrackRelationIsActive = true;
                event->accept();

                return;
            }
        }

        QGraphicsScene::mousePressEvent(event);
    }

    /**
     * @brief Scene::mouseMoveEvent
     * @param event
     */
    void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
    {
        if (showRelationTrack() && m_TrackRelationIsActive) {

            // Check an element under the cursor
            QPointF scenePos = event->scenePos();
            if (auto elem = entElem(this, scenePos)) {

                // Setup second element
                if (elem != m_TrackFrom && elem != m_TrackTo) {
                    setTrackedItemStatus(m_TrackTo, false /*status*/, true /*update*/);
                    m_TrackTo = elem;
                    setTrackedItemStatus(m_TrackTo, true /*status*/, true /*update*/);
                }
            } else {

                // Unselect previous second element
                setTrackedItemStatus(m_TrackTo, false /*status*/, true /*update*/);
                m_TrackTo.clear();
            }

            // Draw tracking line
            QLineF line(m_RelationTrackLine->line());
            line.setP2(scenePos);
            m_RelationTrackLine->setLine(line);

            event->accept();
            return;
        }

        QGraphicsScene::mouseMoveEvent(event);
    }

    /**
     * @brief Scene::mouseReleaseEvent
     * @param event
     */
    void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
    {
        if (showRelationTrack() && m_TrackRelationIsActive) {

            // Hide tracking line and switch off tracking mode
            m_RelationTrackLine->hide();
            m_TrackRelationIsActive = false;

            // Cleanup
            setTrackedItemStatus(m_TrackFrom, false);
            m_TrackFrom.clear();
            setTrackedItemStatus(m_TrackTo, false);
            m_TrackTo.clear();

            event->accept();
            return;
        }

        QGraphicsScene::mouseReleaseEvent(event);
    }

    /**
     * @brief Scene::setShowRelationTrack
     * @param showRelationTrack
     */
    void Scene::setShowRelationTrack(bool showRelationTrack)
    {
        if (m_ShowRelationTrack == showRelationTrack)
            return;

        m_ShowRelationTrack = showRelationTrack;
        if (!m_ShowRelationTrack && m_RelationTrackLine->isVisible())
            m_RelationTrackLine->hide();

        emit showRelationTrackChanged(showRelationTrack);
    }

} // namespace graphics

