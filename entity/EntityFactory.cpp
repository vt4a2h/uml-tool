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
#include "EntityFactory.h"

#include <QGraphicsScene>

#include <commands/movegraphicobject.h>

#include <db/ProjectDatabase.h>

#include <entity/class.h>
#include <entity/extendedtype.h>
#include <entity/union.h>
#include <entity/enum.h>
#include <entity/templateclass.h>

#include <models/projecttreemodel.h>

#include <project/project.h>

namespace entity {

    namespace {
        QHash<KindOfType, std::function<SharedType()>> makers = {
            { KindOfType::Type,          [] { return std::make_shared<entity::Type>();          } },
            { KindOfType::ExtendedType,  [] { return std::make_shared<entity::ExtendedType>();  } },
            { KindOfType::Enum,          [] { return std::make_shared<entity::Enum>();          } },
            { KindOfType::Union,         [] { return std::make_shared<entity::Union>();         } },
            { KindOfType::Class,         [] { return std::make_shared<entity::Class>();         } },
            { KindOfType::TemplateClass, [] { return std::make_shared<entity::TemplateClass>(); } },
            { KindOfType::Structure,
              [] {
                    auto structure = std::make_shared<entity::Class>();
                    structure->setKind(Kind::StructType);
                    return structure;
                 } },
        };
    }

    /**
     * @brief EntitiesFactory::get
     * @return
     */
    const EntityFactory &EntityFactory::instance()
    {
        static EntityFactory instance;
        return instance;
    }

    /**
     * @brief EntityFactory::make
     * @param kindOfType
     * @param pos
     * @param scopeID
     * @param options
     * @return
     */
    SharedType EntityFactory::make(KindOfType kindOfType, const QPointF &pos, const common::ID &scopeID,
                                   CreationOptions options) const
    {
        if (auto database = db()) {
            if (auto scope = database->depthScopeSearch(scopeID)) {
                if (auto maker = makers[kindOfType]) {
                    auto type = maker();

                    if (options.testFlag(AddToScene)) {
                        if (auto s = scene()) {
                            auto graphicEntity = new graphics::Entity(type);
                            graphicEntity->setPos(pos);

                            // Register in the database
                            database->registerGraphicsEntity(graphicEntity);

                            // Transfer ownership
                            s->addItem(graphicEntity);
                        }
                    }

                    if (options.testFlag(AddToTreeModel))
                        if (auto tm = treeModel())
                            if (auto p = project())
                                tm->addType(type, scopeID, p->name());

                    return type;
                }
            }
        }

        return nullptr;
    }

    /**
     * @brief EntityFactory::EntityFactory
     * @param parent
     */
    EntityFactory::EntityFactory(QObject *parent)
        : common::ElementsFactory(parent)
    {
    }

    /**
     * @brief EntityFactory::treeModel
     * @return
     */
    models::SharedTreeModel EntityFactory::treeModel() const
    {
        return m_TreeModel.lock();
    }

    /**
     * @brief EntityFactory::setTreeModel
     * @param treeModel
     */
    void EntityFactory::setTreeModel(const models::WeakTreeModel &treeModel)
    {
        m_TreeModel = treeModel;
    }

//    void EntitiesFactory::connectEntity(graphics::Entity *entity, project::Project *currentProject,
//                                        common::BasicElement */*type*/) const
//    {
//        Q_ASSERT(entity);
//        Q_ASSERT(currentProject);


    // TODO: implement via slot-signals
//        // Connect project
//        G_CONNECT(entity, &graphics::Entity::xChanged, currentProject, &project::Project::touch);
//        G_CONNECT(entity, &graphics::Entity::yChanged, currentProject, &project::Project::touch);
//        G_CONNECT(entity, &graphics::Entity::moved,
//                          [=](const QPointF &from, const QPointF &to) {
//                             auto cmd = std::make_unique<commands::MoveGraphicObject>(
//                                            *entity, entity->typeObject()->name(), from, to);
//                             currentProject->commandsStack()->push(cmd.release());
//                          });
//    }

//    graphics::Entity *EntitiesFactory::newEntity(QGraphicsScene &scene, const QPointF &pos,
//                                                 const entity::SharedType &type) const
//    {
//        graphics::Entity * entity = new graphics::Entity(type);
//        entity->setPos(pos);
//        scene.addItem(entity);

//        return entity;
//    }

} // namespace entity
