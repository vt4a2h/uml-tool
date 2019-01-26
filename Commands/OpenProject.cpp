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

#include <exception>

#include <range/v3/algorithm/find_if.hpp>

#include <QMessageBox>
#include <QMainWindow>

#include <Commands/MakeProjectCurrent.h>

#include <Application/Settings.h>

#include <Project/ProjectDB.hpp>
#include <Project/ProjectFactory.hpp>

namespace Commands
{

    /**
     * @brief OpenProject::OpenProject
     * @param name
     * @param path
     * @param appModel
     * @param stack
     * @param scene
     * @param mv
     * @param rp
     * @param parent
     */
    OpenProject::OpenProject(const QString &name, const QString &path,
                             const Models::SharedApplicationModel &appModel,
                             const Graphics::ScenePtr &scene, QUndoCommand *parent)
        : BaseCommand(name, parent)
        , m_ProjectPath(path)
        , m_AppModel(appModel)
        , m_Scene(scene)
    {
    }

    /**
     * @brief OpenProject::redo
     */
    void OpenProject::redoImpl()
    {
        try {
            if (m_Done)
                throw std::logic_error(tr("Operation is not permitted").toUtf8());

            auto projects = m_AppModel->projectsDb().projectsAsVector();
            auto it = ranges::find_if(projects, [&](auto &&p) { return p->fullPath() == m_ProjectPath; });
            m_Project = it != std::end(projects) ? *it : nullptr;
            if (m_Project)
                throw std::logic_error(tr("Cannot add the same project twice").toUtf8());

            m_Project = Projects::ProjectFactory::instance().makeProject();
            m_Project->load(m_ProjectPath);

            if (m_Project->hasErrors())
                throw std::logic_error(m_Project->lastErrors().join("\n").toUtf8());

            if (!m_AppModel->projectsDb().addProject(m_Project))
                throw std::logic_error(tr("Cannot add project with the same name twice").toUtf8());

            Commands::MakeProjectCurrent(m_Project->name(), m_AppModel, m_Scene).redo();

            emit recentProjectAdded(m_Project->fullPath());

            m_Done = true;
        } catch (const std::logic_error &e) {
            setObsolete(true);
            emit projectErrors(QString::fromUtf8(e.what()));
        } catch (...) {
            Q_ASSERT(!"Unexpected error");
        }
    }

    /**
     * @brief OpenProject::undo
     */
    void OpenProject::undoImpl()
    {
        Q_ASSERT(!"Command is not undoable");
        setObsolete(true);
    }

} // namespace Commands
