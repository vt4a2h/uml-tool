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
#include "entitiesfactory.h"

#include <QGraphicsScene>

#include <commands/movegraphicobject.h>

#include "class.h"

namespace entity {

    /**
     * @brief EntitiesFactory::get
     * @return
     */
    const EntitiesFactory &EntitiesFactory::get()
    {
        static EntitiesFactory instance;
        return instance;
    }

    /**
     * @brief EntitiesFactory::addEntity
     * @param scene
     * @param type
     * @param pos
     */
    graphics::Entity *EntitiesFactory::addEntity(QGraphicsScene &scene, const project::SharedProject &project,
                                                 const SharedType &type, const QPointF &pos) const
    {
        graphics::Entity * entity = newEntity(scene, pos, type);
        connectEntity(entity, project.get(), type.get());
        return entity;
    }

    /**
     * @brief EntitiesFactory::EntitiesFactory
     */
    EntitiesFactory::EntitiesFactory()
    {
    }

    void EntitiesFactory::connectEntity(graphics::Entity *entity, project::Project *currentProject,
                                        BasicEntity *type) const
    {
        Q_ASSERT(entity);
        Q_ASSERT(currentProject);
        Q_ASSERT(type);

        // Connect project
        G_CONNECT(entity, &graphics::Entity::xChanged, currentProject, &project::Project::touch);
        G_CONNECT(entity, &graphics::Entity::yChanged, currentProject, &project::Project::touch);
        G_CONNECT(entity, &graphics::Entity::moved,
                          [=](const QPointF &from, const QPointF &to) {
                             auto cmd = std::make_unique<commands::MoveGraphicObject>(
                                            *entity, entity->typeObject()->name(), from, to);
                             currentProject->commandsStack()->push(cmd.release());
                          });
    }

    graphics::Entity *EntitiesFactory::newEntity(QGraphicsScene &scene, const QPointF &pos,
                                                 const entity::SharedType &type) const
    {
        graphics::Entity * entity = new graphics::Entity(type);
        entity->setPos(pos);
        scene.addItem(entity);

        return entity;
    }

} // namespace entity
