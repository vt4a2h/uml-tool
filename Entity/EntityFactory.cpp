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

#include <functional>

#include <QGraphicsScene>

#include <DB/ProjectDatabase.h>
#include <Entity/Class.h>
#include <Entity/ExtendedType.h>
#include <Entity/Union.h>
#include <Entity/Enum.h>
#include <Entity/TemplateClass.h>

#include <Models/ProjectTreeModel.h>
#include <Project/Project.h>

namespace Entity {

    namespace {
        QHash<KindOfType, std::function<SharedType(const Entity::SharedScope &)>> makers = {
            { KindOfType::Type,
              [] (const Entity::SharedScope &s) { return s->addType<Entity::Type>();          } },
            { KindOfType::ExtendedType,
              [] (const Entity::SharedScope &s) { return s->addType<Entity::ExtendedType>();  } },
            { KindOfType::Enum,
              [] (const Entity::SharedScope &s) { return s->addType<Entity::Enum>();          } },
            { KindOfType::Union,
              [] (const Entity::SharedScope &s) { return s->addType<Entity::Union>();         } },
            { KindOfType::Class,
              [] (const Entity::SharedScope &s) { return s->addType<Entity::Class>();         } },
            { KindOfType::TemplateClass,
              [] (const Entity::SharedScope &s) { return s->addType<Entity::TemplateClass>(); } },
        };

        void addGraphicEntity(const QPointer<QGraphicsScene> &scene,
                              const DB::SharedProjectDatabase &projectDB,
                              const Entity::SharedType &type,
                              const QPointF pos = QPointF())
        {
            if (scene && projectDB) {
                auto graphicEntity = new Graphics::GraphisEntity(type);

                // Register in the database
                projectDB->registerGraphicsEntity(graphicEntity);

                // Transfer ownership
                scene->addItem(graphicEntity);

                // Set position only after adding item in order to appropriate event will be generated
                // and signal about changing position will be emitted
                if (!pos.isNull())
                    graphicEntity->setPos(pos);
            }
        }
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
    SharedType EntityFactory::make(KindOfType kindOfType, const QPointF &pos,
                                   const Common::ID &scopeID,
                                   CreationOptions options) const
    {
        if (auto currentScope = scope(scopeID)) {
            if (auto maker = makers[kindOfType]) {
                auto type = maker(currentScope);
                if (!type)
                    return nullptr;

                // Will be added by default
                Q_ASSERT(!options.testFlag(AddToDatabase));

                if (project() && options.testFlag(AddToScene))
                    addGraphicEntity(scene(), project()->database(), type, pos);

                if (project() && options.testFlag(AddToTreeModel))
                    if (auto tm = treeModel())
                        if (auto p = project())
                            tm->addType(type, scopeID, p->name());

                return type;
            }
        }

        return nullptr;
    }

    /**
     * @brief EntityFactory::make
     * @param src
     * @param errors
     * @param scopeID
     * @param options
     * @return
     */
    SharedType EntityFactory::make(const QJsonObject &src, ErrorList &errors, const Common::ID &scopeID,
                                   Common::ElementsFactory::CreationOptions options) const
    {
        if (src.contains(Entity::Type::kindMarker())) {
            auto kind = KindOfType(src[Entity::Type::kindMarker()].toInt());

            // Add to scene manually later
            bool addToScene = false;
            if (options.testFlag(AddToScene)) {
                options ^= AddToScene;
                addToScene = true;
            }

            if (auto result = make(kind, QPointF(0, 0), scopeID, options)) {
                result->fromJson(src, errors);
                if (errors.isEmpty()) {
                    if (addToScene && project())
                        addGraphicEntity(scene(), project()->database(), result);

                    return result;
                }
            } else {
                errors << "Cannot create object.";
            }

        }

        return nullptr;
    }

    /**
     * @brief EntityFactory::EntityFactory
     * @param parent
     */
    EntityFactory::EntityFactory(QObject *parent)
        : Common::ElementsFactory(parent)
    {
    }

} // namespace entity
