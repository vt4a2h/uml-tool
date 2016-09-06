/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 17/01/2016.
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

#include <memory>

#include <QPointer>

QT_BEGIN_NAMESPACE
class QGraphicsItem;
class QGraphicsScene;
QT_END_NAMESPACE

namespace common { class ID; }

namespace graphics {

    class Entity;
    using EntityPtr = QPointer<Entity>;
    using EntityHashMap = QHash<common::ID, EntityPtr>;
    using EntityVector = QVector<EntityPtr>;

    class Relation;
    using RelationPtr = QPointer<Relation>;
    using UniqueGraphicRelation = std::unique_ptr<Relation>;
    using GraphicRelationPtr = QPointer<Relation>;
    using RelationHashMap = QHash<common::ID, RelationPtr>;
    using RelationVector = QVector<GraphicRelationPtr>;

    using GraphicItems = QList<QGraphicsItem*>;
    using ScenePtr = QPointer<QGraphicsScene>;

    class Scene;
    using UniqueScene = std::unique_ptr<Scene>;

} // namespace graphics

