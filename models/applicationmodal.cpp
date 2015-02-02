/*****************************************************************************
**
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 01/26/2015.
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
#include "applicationmodal.h"

#include <project/project.h>

#include "projecttreemodel.h"

namespace models {

    /**
     * @brief ApplicationModal::ApplicationModal
     * @param parent
     */
    ApplicationModal::ApplicationModal(QObject *parent)
        : QObject(parent)
        , m_TreeModel(std::make_shared<ProjectTreeModel>(m_Projects))
    {
        // make some test data
        makeProject("Project1", "/path1");
        makeProject("project2", "/path2");
        makeProject("project3", "/path3");

        m_TreeModel->fillData();
    }

    /**
     * @brief ApplicationModal::~ApplicationModal
     */
    ApplicationModal::~ApplicationModal()
    {
    }

    /**
     * @brief ApplicationModal::makeProject
     * @param name
     * @param path
     * @return
     */
    project::SharedProject ApplicationModal::makeProject(const QString &name, const QString &path)
    {
        project::SharedProject newProject(std::make_shared<project::Project>(name, path));
        return *m_Projects.insert(newProject->id(), newProject);
    }

    /**
     * @brief ApplicationModal::projects
     * @return
     */
    project::ProjectsList ApplicationModal::projects() const
    {
        return m_Projects.values();
    }

    /**
     * @brief ApplicationModal::removeProject
     * @param id
     */
    bool ApplicationModal::removeProject(const QString &id)
    {
        return !!m_Projects.remove(id);
    }

    /**
     * @brief ApplicationModal::containsProject
     * @param id
     * @return
     */
    bool ApplicationModal::containsProject(const QString &id)
    {
        return m_Projects.contains(id);
    }

    /**
     * @brief ApplicationModal::currentProject
     * @return
     */
    project::SharedProject ApplicationModal::currentProject() const
    {
        return m_CurrentProject;
    }

    /**
     * @brief ApplicationModal::setCurrentProject
     * @param id
     * @return
     */
    bool ApplicationModal::setCurrentProject(const QString &id)
    {
        if (!m_Projects.contains(id))
            return false;

        m_CurrentProject = m_Projects[id];
        return true;
    }

    /**
     * @brief ApplicationModal::treeModel
     * @return
     */
    SharedTreeModel ApplicationModal::treeModel() const
    {
        return m_TreeModel;
    }

} // namespace models
