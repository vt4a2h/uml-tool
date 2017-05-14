/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 10/05/2015.
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

#include <QHash>
#include <QMetaType>

namespace Common { class ID; }

namespace Relationship {
    class Node;
    using SharedNode = std::shared_ptr<Node>;

    class Relation;
    using SharedRelation = std::shared_ptr<Relation>;
    using Relations = QHash<Common::ID, SharedRelation>;
    using RelationsList = QVector<SharedRelation>;

    class AddRelation;
    using UniqueRelationCmd = std::unique_ptr<AddRelation>;
}

Q_DECLARE_METATYPE(Relationship::SharedRelation)
