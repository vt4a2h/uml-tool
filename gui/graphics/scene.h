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
#pragma once

#include <QGraphicsScene>
#include <QPointer>

#include <project/project_types.hpp>

namespace graphics {

    class Entity;

    /// The main scene
    class Scene : public QGraphicsScene
    {
        Q_OBJECT
        Q_PROPERTY(bool showRelationTrack READ showRelationTrack WRITE setShowRelationTrack
                   NOTIFY showRelationTrackChanged)
    public:
        Scene(QObject * parent = nullptr);
        ~Scene();

        void initTrackLine();

        bool showRelationTrack() const;

        static int elementTypeKey();

    public: // QGraphicsScene overrides
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    public slots:
        void setShowRelationTrack(bool showRelationTrack);
        void onProjectChanged(const project::SharedProject &p);

    signals:
        void showRelationTrackChanged(bool showRelationTrack);
        void relationCompleted();

    private:
        project::SharedProject pr() const;

        bool m_ShowRelationTrack;
        bool m_TrackRelationIsActive;
        QPointer<Entity> m_TrackFrom;
        QPointer<Entity> m_TrackTo;

        QGraphicsLineItem * m_RelationTrackLine;

        project::WeakProject m_Project;
    };

} // namespace grphics
