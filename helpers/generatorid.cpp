/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 17/01/2016.
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
#include "generatorid.h"

#include <project/project.h>

namespace entity {

    /**
     * @brief GeneratorID::instance
     * @return
     */
    const GeneratorID &GeneratorID::instance()
    {
        static GeneratorID instance;
        return instance;
    }

    /**
     * @brief GeneratorID::genID
     * @return
     */
    EntityID GeneratorID::genID()
    {
        auto pr = project();
        return pr ? pr->genID() : nullID();
    }

    /**
     * @brief GeneratorID::defaultID
     * @return
     */
    entity::EntityID GeneratorID::nullID()
    {
        return 0;
    }

    /**
     * @brief Returns first free IF for for project items. ID's from 0 to 4096 are reserved
     *        for Qt and C++ types. Can be used to init project id counter or for sanity check.
     * @return first free ID.
     */
    EntityID GeneratorID::firstFreeID()
    {
        return 4097;
    }

    /**
     * @brief GeneratorID::onCurrentProjectChanged
     * @param p
     */
    void GeneratorID::onCurrentProjectChanged(const project::SharedProject &p)
    {
        m_CurrentProject = p;
    }

    /**
     * @brief GeneratorID::GeneratorID
     * @param parent
     */
    GeneratorID::GeneratorID(QObject *parent)
        : QObject(parent)
    {
    }

    /**
     * @brief GeneratorID::project
     * @return
     */
    project::SharedProject GeneratorID::project() const
    {
        return m_CurrentProject.lock();
    }

} // entity
