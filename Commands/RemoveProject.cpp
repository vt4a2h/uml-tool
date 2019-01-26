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

#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/algorithm/equal.hpp>

#include <QGraphicsScene>
#include <QStringBuilder>

#include <GUI/graphics/GraphicsRelation.h>
#include <GUI/graphics/Entity.h>

#include <Project/ProjectDB.hpp>

namespace Commands {

   /**
     * @brief RemoveProject::RemoveProject
     * @param p
     * @param m
     */
    RemoveProject::RemoveProject(const Projects::SharedProject &p,
                                 const Models::SharedApplicationModel &a,
                                 const Graphics::ScenePtr &scene)
        : BaseCommand(tr("Remove project \"") % G_ASSERT(p)->name() % "\"")
        , m_Project(p)
        , m_AppModel(G_ASSERT(a))
        , m_Scene(G_ASSERT(scene))
    {
    }

    /**
     * @brief RemoveProject::undo
     */
    void RemoveProject::undoImpl()
    {
        // Restore project
        G_ASSERT(m_AppModel->projectsDb().addProject(m_Project));

        // Recover project status
        if (m_WasCurrent)
            m_AppModel->setCurrentProject(m_Project->name());

        // Add graphics components
        ranges::for_each(m_GraphicItems, [this](auto &&i) { G_ASSERT(m_Scene)->addItem(i); });
    }

    /**
     * @brief RemoveProject::redo
     */
    void RemoveProject::redoImpl()
    {
        if (!m_Done) {
            m_GraphicItems = m_Project->database()->graphicsItems();
            m_Done = true;
        }

        sanityCheck();

        // Reset current project
        if (m_AppModel->currentProject() == m_Project) {
            m_AppModel->setCurrentProject("");
            m_WasCurrent = true;
        }

        // Remove project itself
        G_ASSERT(m_AppModel)->projectsDb().removeProject(m_Project);

        // Clean scene
        ranges::for_each(m_GraphicItems, [this](auto &&i) { G_ASSERT(m_Scene)->removeItem(i); });
    }

    /**
     * @brief RemoveProject::sanityCheck
     */
    void RemoveProject::sanityCheck()
    {
        Q_ASSERT_X(ranges::equal(m_Project->database()->graphicsItems(), m_GraphicItems),
                   Q_FUNC_INFO, "Project is in the inconsistent state.");

        Q_ASSERT_X(!m_Scene.isNull() || (m_Scene.isNull() && m_GraphicItems.isEmpty()),
                   Q_FUNC_INFO, "Invalid scene");
    }

} // namespace Commands
