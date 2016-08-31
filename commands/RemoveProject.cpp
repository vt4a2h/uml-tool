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
#include <boost/range/algorithm/equal.hpp>

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
        : m_Project(G_ASSERT(p))
        , m_AppModel(G_ASSERT(a))
    {
    }

    /**
     * @brief RemoveProject::undo
     */
    void RemoveProject::undo()
    {
        m_AppModel->addProject(m_Project);

        if (m_WasCurrent)
            m_AppModel->setCurrentProject(m_Project);
    }

    /**
     * @brief RemoveProject::redo
     */
    void RemoveProject::redo()
    {
        if (!m_Done) {
            G_ASSERT(m_AppModel)->removeProject(G_ASSERT(m_Project)->name());
            m_GraphicItems = m_Project->database()->graphicsItems();
            m_Done = true;
        }

        sanityCheck();

        // Clean scene
        boost::range::for_each(m_GraphicItems, [](auto &&i) { G_ASSERT(i->scene())->removeItem(i); });

        if (m_AppModel->currentProject() == m_Project) {
            // Reset current project
            m_AppModel->setCurrentProject("");
            m_WasCurrent = true;
        }

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

    /**
     * @brief RemoveProject::sanityCheck
     */
    void RemoveProject::sanityCheck()
    {
        Q_ASSERT_X(boost::range::equal(m_Project->database()->graphicsItems(), m_GraphicItems),
                   Q_FUNC_INFO, "Project is in the inconsistent state.");
    }

} // namespace commands
