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

#include <entity/Class.h>
#include <entity/ExtendedType.h>
#include <entity/Union.h>
#include <entity/Enum.h>
#include <entity/TemplateClass.h>
#include <entity/Scope.h>

#include <models/ProjectTreeModel.h>

#include <project/Project.h>

namespace entity {

    namespace {
        QHash<KindOfType, std::function<SharedType(const entity::SharedScope &)>> makers = {
            { KindOfType::Type,
              [] (const entity::SharedScope &s) { return s->addType<entity::Type>();          } },
            { KindOfType::ExtendedType,
              [] (const entity::SharedScope &s) { return s->addType<entity::ExtendedType>();  } },
            { KindOfType::Enum,
              [] (const entity::SharedScope &s) { return s->addType<entity::Enum>();          } },
            { KindOfType::Union,
              [] (const entity::SharedScope &s) { return s->addType<entity::Union>();         } },
            { KindOfType::Class,
              [] (const entity::SharedScope &s) { return s->addType<entity::Class>();         } },
            { KindOfType::TemplateClass,
              [] (const entity::SharedScope &s) { return s->addType<entity::TemplateClass>(); } },
        };

        void addGraphicEntity(const QPointer<QGraphicsScene> &scene,
                              const db::SharedProjectDatabase &projectDB,
                              const entity::SharedType &type,
                              const QPointF pos = QPointF())
        {
            if (G_ASSERT(scene) && G_ASSERT(projectDB)) {
                auto graphicEntity = new graphics::Entity(type);

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
                                   const common::ID &scopeID,
                                   CreationOptions options) const
    {
        if (auto database = db()) {
            if (auto scope = database->depthScopeSearch(scopeID)) {
                if (auto maker = makers[kindOfType]) {
                    auto type = maker(scope);
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
    SharedType EntityFactory::make(const QJsonObject &src, ErrorList &errors, const common::ID &scopeID,
                                   common::ElementsFactory::CreationOptions options) const
    {
        if (src.contains(entity::Type::kindMarker())) {
            auto kind = KindOfType(src[entity::Type::kindMarker()].toInt());

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
        : common::ElementsFactory(parent)
    {
    }

} // namespace entity
