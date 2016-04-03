/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 13/03/2015.
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

#include <common/ElementsFactory.h>

#include "entity_types.hpp"

#include <entity/type.h>

#include <gui/graphics/entity.h>

#include <models/models_types.hpp>


class QGraphicsScene;

namespace entity {

    /// The EntitiesFactory class
    class EntityFactory : public common::ElementsFactory
    {
    public:
        EntityFactory(const EntityFactory &) = delete;
        EntityFactory(EntityFactory &&) = delete;
        EntityFactory& operator =(const EntityFactory&) = delete;
        EntityFactory& operator =(EntityFactory&&) = delete;

        static const EntityFactory &instance();

        SharedType make(KindOfType kindOfType, const QPointF &pos = QPointF(0, 0),
                        const common::ID &scopeID = common::ID::projectScopeID(),
                        CreationOptions options = AddToScene | AddToTreeModel) const;
        SharedType make(const QJsonObject &src, ErrorList &errors,
                        const common::ID &scopeID = common::ID::projectScopeID(),
                        CreationOptions options = AddToScene | AddToTreeModel) const;

        models::SharedTreeModel treeModel() const;
        void setTreeModel(const models::WeakTreeModel &treeModel);

    private:
        explicit EntityFactory(QObject * parent = nullptr);

        models::WeakTreeModel m_TreeModel;
    };

} // namespace entity
