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

#include "entity_types.hpp"

#include <project/project_types.hpp>

#include <gui/graphics/entity.h>

#include <models/applicationmodel.h>
#include <models/models_types.hpp>

class QGraphicsScene;

namespace entity {

    /// The EntitiesFactory class
    class EntitiesFactory
    {
    public:
        static const EntitiesFactory &get();

        template<class Type>
        std::pair<std::shared_ptr<Type>, graphics::Entity*>
        makeEntity(const models::SharedApplicationModel &model, const entity::EntityID  &scopeID,
                   QGraphicsScene &scene, const QPointF &pos) const
        {
            auto type = model->makeType<Type>(scopeID);
            type->setName(type->defaultName());

            auto &&currentProject = model->currentProject();

            graphics::Entity * entity = newEntity(scene, pos, type);
            connectEntity(entity, currentProject.get());

            currentProject->touch();

            return {type, entity};
        }


        graphics::Entity *addEntity(QGraphicsScene &scene, const project::SharedProject &project,
                                    const entity::SharedType &type, const QPointF &pos) const;

    private:
        EntitiesFactory();
        EntitiesFactory(const EntitiesFactory &);
        EntitiesFactory &operator=(const EntitiesFactory &);

        graphics::Entity *newEntity(QGraphicsScene &scene, const QPointF &pos,
                                    const entity::SharedType &type = nullptr) const;

        void connectEntity(graphics::Entity * entity, project::Project * currentProject) const;
    };

} // namespace entity
