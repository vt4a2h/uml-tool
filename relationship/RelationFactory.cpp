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
#include "RelationFactory.h"

#include <QGraphicsScene>

#include <db/Database.h>
#include <db/ProjectDatabase.h>

#include <project/Project.h>

#include <gui/graphics/GraphicsRelation.h>
#include <gui/graphics/Entity.h>

#include <utility/helpfunctions.h>

#include "Relation.h"
#include "multiplyassociation.h"
#include "association.h"
#include "dependency.h"
#include "realization.h"
#include "qthelpers.h"

namespace relationship {

    namespace {
        using MakerR  = std::function<SharedRelation(const common::ID &, const common::ID &,
                                                     const db::WeakTypeSearchers &)>;
        using MakersR = const QMap<RelationType, MakerR>;

        MakersR relationMaker = {
            {AggregationRelation,    [](auto &&tail, auto &&head, auto &&searchers){
                 auto rel =
                     std::make_shared<relationship::MultiplyAssociation>(tail, head, searchers);
                 rel->setRelationType(AggregationRelation);
                 return rel;
             }},
            {CompositionRelation,    [](auto &&tail, auto &&head, auto &&searchers){
                 auto rel =
                     std::make_shared<relationship::MultiplyAssociation>(tail, head, searchers);
                 rel->setRelationType(CompositionRelation);
                 return rel;
             }},
            {MultiRelation,          [](auto &&tail, auto &&head, auto &&searchers){
                 return std::make_shared<MultiplyAssociation>(tail, head, searchers); }},
            {AssociationRelation,    [](auto &&tail, auto &&head, auto &&searchers){
                 return std::make_shared<Association>(tail, head, searchers);         }},
            {DependencyRelation ,    [](auto &&tail, auto &&head, auto &&searchers){
                 return std::make_shared<Dependency>(tail, head, searchers);          }},
            {GeneralizationRelation, [](auto &&tail, auto &&head, auto &&searchers){
                 return std::make_shared<Generalization>(tail, head, searchers);      }},
            {RealizationRelation,    [](auto &&tail, auto &&head, auto &&searchers){
                 return std::make_shared<Realization>(tail, head, searchers);         }},
            {SimpleRelation,         [](auto &&tail, auto &&head, auto &&searchers){
                 return std::make_shared<Relation>(tail, head, searchers);            }}
        };
    }

    /**
     * @brief RelationFactory::instance
     * @return
     */
    const RelationFactory &RelationFactory::instance()
    {
        static RelationFactory instance;
        return instance;
    }

    /**
     * @brief RelationFactory::make
     * @param relType
     * @param tail
     * @param head
     * @param addToScene
     * @return
     */
    SharedRelation RelationFactory::make(RelationType relType, const common::ID &tail,
                                         const common::ID &head, CreationOptions options) const
    {
        if (auto pr = G_ASSERT(project())) {
            if (G_ASSERT(pr->database() && project()->globalDatabase())) {
                if (auto maker = G_ASSERT(relationMaker[relType])) {
                    if (auto relation = maker(tail, head, {pr->database(), pr->globalDatabase()})) {

                        auto projectDB = project()->database();

                        if (options.testFlag(ElementsFactory::AddToDatabase))
                            projectDB->addRelation(relation);

                        if (options.testFlag(ElementsFactory::AddToScene)) {
                            if (auto graphicsScene = scene()) {
                                auto from = G_ASSERT(projectDB->graphicsEntity(tail));
                                auto to   = G_ASSERT(projectDB->graphicsEntity(head));

                                auto graphicsRelation =
                                    std::make_unique<graphics::Relation>(relation, from, to);
                                graphicsScene->addItem(graphicsRelation.release());

                                // FIXME: register relation in the database
                            }
                        }

                        if (options.testFlag(ElementsFactory::AddToTreeModel)) {
                            // TODO: implement
                        }
                    }
                }
            }
        }

        return nullptr;
    }

    /**
     * @brief RelationFactory::make
     * @param src
     * @param addToScene
     * @return
     */
    SharedRelation RelationFactory::make(const QJsonObject &src, CreationOptions options) const
    {

    }

    /**
     * @brief RelationFactory::makeCmd
     * @param relType
     * @param tail
     * @param head
     * @return
     */
    UniqueRelationCmd RelationFactory::makeCmd(RelationType relType,
                                               const entity::SharedType &tail,
                                               const entity::SharedType &head) const
    {

    }

    /**
     * @brief RelationFactory::RelationFactory
     * @param parent
     */
    RelationFactory::RelationFactory(QObject *parent)
        : common::ElementsFactory(parent)
    {
    }

} // namespace relationship
