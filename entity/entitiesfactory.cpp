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
     * @brief EntitiesFactory::makeClass
     * @param model
     * @param scopeID
     * @param scene
     * @return
     */
    SharedClass EntitiesFactory::makeClass(const models::SharedApplicationModal &model,
                                           const QString &scopeID, QGraphicsScene &scene) const
    {
        Q_UNUSED(model);
        Q_UNUSED(scopeID);
        Q_UNUSED(scene);

        return SharedClass();
    }

    /**
     * @brief EntitiesFactory::EntitiesFactory
     */
    EntitiesFactory::EntitiesFactory()
    {
    }

} // namespace entity
