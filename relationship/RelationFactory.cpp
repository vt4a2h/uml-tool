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

namespace relationship {

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
                                         const common::ID &head, bool addToScene) const
    {

    }

    /**
     * @brief RelationFactory::make
     * @param src
     * @param addToScene
     * @return
     */
    SharedRelation RelationFactory::make(const QJsonObject &src, bool addToScene) const
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
