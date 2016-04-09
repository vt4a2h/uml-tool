/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 31/03/2016.
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
#include "ElementsFactory.h"

#include <project/Project.h>

namespace common {

    /**
     * @brief ElementsFactory::ElementsFactory
     * @param parent
     */
    ElementsFactory::ElementsFactory(QObject *parent) : QObject(parent)
    {
    }

    /**
     * @brief ElementsFactory::onSceneChanged
     * @param scene
     */
    void ElementsFactory::onSceneChanged(const QPointer<QGraphicsScene> &scene)
    {
        m_Scene = scene;
    }

    /**
     * @brief ElementsFactory::onProjectChanged
     * @param p
     */
    void ElementsFactory::onProjectChanged(const project::SharedProject &p)
    {
        m_Project = p;
    }

    /**
     * @brief ElementsFactory::scene
     * @return
     */
    QPointer<QGraphicsScene> ElementsFactory::scene() const
    {
        return m_Scene;
    }

    /**
     * @brief ElementsFactory::db
     * @return
     */
    db::SharedProjectDatabase ElementsFactory::db() const
    {
        return project() ? project()->database() : nullptr;
    }

    /**
     * @brief ElementsFactory::project
     * @return
     */
    project::SharedProject ElementsFactory::project() const
    {
        return m_Project.lock();
    }

} // namespace common
