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
#include <entity/scope.h>

#include <models/projecttreemodel.h>

#include <project/project.h>

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
            if (auto result = make(kind, QPointF(0, 0), scopeID, options)) {
                result->fromJson(src, errors);
                if (errors.isEmpty())
                    return result;
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

} // namespace entity
