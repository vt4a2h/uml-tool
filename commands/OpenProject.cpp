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

#include <functional>

#include <boost/range/algorithm/find_if.hpp>

#include <QMessageBox>
#include <QMainWindow>

#include <commands/MakeProjectCurrent.h>

#include <project/Project.h>

#include "QtHelpers.h"

namespace Commands
{

    using namespace boost;

    OpenProject::OpenProject(const QString &name, const QString &path,
                             const models::SharedApplicationModel &appModel,
                             const Commands::SharedCommandStack &stack,
                             const graphics::ScenePtr &scene,
                             QMainWindow &mv, QMenu &rp, QUndoCommand *parent)
        : BaseCommand(name, parent)
        , m_ProjectPath(path)
        , m_AppModel(appModel)
        , m_CommandsStack(stack)
        , m_Scene(scene)
        , m_MainWindow(mv)
        , m_RecentProjectsMenu(rp)
        , m_SupressDialogs(false)
    {
    }

    /**
     * @brief OpenProject::redo
     */
    void OpenProject::redo()
    {
        sanityCheck();

        if (!m_Done) {
            auto projects = m_AppModel->projects();
            auto it = range::find_if(projects, [&](auto &&p) { return p->fullPath() == m_ProjectPath; });
            m_Project = it != std::end(projects) ? *it : nullptr;

            if (!m_Project) {
                m_Project = std::make_shared<Projects::Project>();
                m_Project->load(m_ProjectPath);
            }

            m_Done = true;
        }

        // Just in case
        if (!G_ASSERT(m_Project))
            return;

        if (m_Project->hasErrors()) {
            if (!m_SupressDialogs) {
                auto errors = m_Project->lastErrors();
                QMessageBox::critical
                    ( &m_MainWindow
                    , tr("Open project: %n error(s).", "", errors.count())
                    , errors.join("\n")
                    , QMessageBox::Ok
                    );
            }

            return;
        }

        m_AppModel->addProject(m_Project);
        m_Project->setCommandsStack(m_CommandsStack);

        if (m_Project != m_AppModel->currentProject() && !m_MakeProjectCurrentCmd)
            m_MakeProjectCurrentCmd =
                std::make_shared<Commands::MakeProjectCurrent>("", m_AppModel, m_Scene);

        if (m_MakeProjectCurrentCmd)
            m_MakeProjectCurrentCmd->redo();

        m_RecentProjectsAdder(m_Project->fullPath());
        m_MenuRebuilder(m_RecentProjectsMenu);
    }

    /**
     * @brief OpenProject::undo
     */
    void OpenProject::undo()
    {
        sanityCheck();

        if (!m_Project)
            return;

        if (m_MakeProjectCurrentCmd)
            m_MakeProjectCurrentCmd->undo();

        m_AppModel->removeProject(m_Project->name());
    }

    /**
     * @brief OpenProject::sanityCheck
     */
    void OpenProject::sanityCheck()
    {
        Q_ASSERT(m_RecentProjectsAdder && m_MenuRebuilder);
        Q_ASSERT(!m_ProjectPath.isEmpty());
    }

    /**
     * @brief OpenProject::setProjectAdder
     * @param ProjectAdder
     */
    void OpenProject::setProjectAdder(const ProjectAdder &projectAdder)
    {
        m_RecentProjectsAdder = projectAdder;
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
     * @brief OpenProject::supressDialogs
     * @return
     */
    bool OpenProject::supressDialogs() const
    {
        return m_SupressDialogs;
    }

    /**
     * @brief OpenProject::setSupressDialogs
     * @param supressDialogs
     */
    void OpenProject::setSupressDialogs(bool supressDialogs)
    {
        m_SupressDialogs = supressDialogs;
    }

} // Commands
