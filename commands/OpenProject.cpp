/*****************************************************************************
**
** Copyright (C) 2017 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 23.
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
#include "OpenProject.h"

namespace Commands
{

    OpenProject::OpenProject(const QString &name, const QString &path,
                             const models::SharedApplicationModel &appModel,
                             const Commands::SharedCommandStack &stack, graphics::Scene &scene,
                             QMainWindow &mv, QMenu &rp, QUndoCommand *parent)
        : BaseCommand(name, parent)
        , m_ProjectPath(path)
        , m_AppModel(appModel)
        , m_CommandsStack(stack)
        , m_Scene(scene)
        , m_MainWindow(mv)
        , m_RecentProjectsMenu(rp)
    {
    }

    /**
     * @brief OpenProject::redo
     */
    void OpenProject::redo()
    {
        sanityCheck();
    }

    /**
     * @brief OpenProject::sanityCheck
     */
    void OpenProject::sanityCheck()
    {
        Q_ASSERT(m_ProjectAdder && m_MenuRebuilder);
    }

    /**
     * @brief OpenProject::cleanup
     */
    void OpenProject::cleanup()
    {

    }

    /**
     * @brief OpenProject::setProjectAdder
     * @param ProjectAdder
     */
    void OpenProject::setProjectAdder(const ProjectAdder &projectAdder)
    {
        m_ProjectAdder = projectAdder;
    }

    /**
     * @brief OpenProject::setMenuRebuilder
     * @param MenuRebuilder
     */
    void OpenProject::setMenuRebuilder(const MenuRebuilder &menuRebuilder)
    {
        m_MenuRebuilder = menuRebuilder;
    }

    /**
     * @brief OpenProject::undo
     */
    void OpenProject::undo()
    {
        sanityCheck();
    }

} // Commands
