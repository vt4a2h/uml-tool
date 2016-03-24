/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 24/03/2016.
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

#include <QPointer>

#include <db/db_types.hpp>
#include <relationship/relationship_types.hpp>
#include <entity/entity_types.hpp>

#include "enums.h"

class QGraphicsScene;

namespace relationship {

    /// Relation maker
    class RelationFactory : public QObject
    {
        Q_OBJECT

    public:
        RelationFactory(const RelationFactory &) = delete;
        RelationFactory(RelationFactory &&) = delete;
        RelationFactory& operator =(const RelationFactory&) = delete;
        RelationFactory& operator =(RelationFactory&&) = delete;

        static const RelationFactory &instance();

        SharedRelation make(RelationType relType, const common::ID &tail,
                            const common::ID &head, bool addToScene = true) const;
        SharedRelation make(const QJsonObject &src, bool addToScene = true) const;

        UniqueRelationCmd makeCmd(RelationType relType, const entity::SharedType &tail,
                                  const entity::SharedType &head) const;

        QPointer<QGraphicsScene> scene() const;

    public slots:
        void onDbChanged(const db::SharedProjectDatabase &newDb);
        void setSceneChanged(const QPointer<QGraphicsScene> &scene);

    private:
        explicit RelationFactory(QObject * parent = nullptr);

        QPointer<QGraphicsScene> m_Scene;
        db::WeakProjectDatabase m_Db;
    };

} // namespace relationship
