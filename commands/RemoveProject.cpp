/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 30/08/2016.
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
#include "RemoveProject.h"

#include <QGraphicsScene>

#include <boost/range/algorithm/for_each.hpp>

#include <gui/graphics/GraphicsRelation.h>
#include <gui/graphics/Entity.h>

namespace commands {

    /**
     * @brief RemoveProject::RemoveProject
     * @param p
     * @param m
     */
    RemoveProject::RemoveProject(const project::SharedProject &p,
                                 const models::SharedApplicationModel &a)
        : m_Project(p)
        , m_AppModel(a)
    {
    }

    /**
     * @brief RemoveProject::undo
     */
    void RemoveProject::undo()
    {
        m_AppModel->addProject(m_Project);
        // TODO: add commad to make project curent
    }

    /**
     * @brief RemoveProject::redo
     */
    void RemoveProject::redo()
    {
        G_ASSERT(m_AppModel)->removeProject(G_ASSERT(m_Project)->name());
        m_GraphicItems = m_Project->database()->graphicsItems();
        boost::range::for_each(m_GraphicItems, [](auto &&i) { G_ASSERT(i->scene())->removeItem(i); });

        m_CleaningRequired = true;
    }

    /**
     * @brief RemoveProject::cleanup
     */
    void RemoveProject::cleanup()
    {
        if (m_CleaningRequired)
            qDeleteAll(m_GraphicItems);
    }

} // namespace commands
