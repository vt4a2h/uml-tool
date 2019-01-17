/*****************************************************************************
**
** Copyright (C) 2019 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 15/1/2019.
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
#include "ProjectDB.hpp"

#include <Project/Project.h>

namespace Projects {

    ProjectDatabase::ProjectDatabase() = default;

    /**
     * @brief ProjectDatabase::addProject
     * @param project
     */
    void ProjectDatabase::addProject(const SharedProject &project)
    {
        if (project && !m_Projects.contains(project->name())) {
            m_Projects[project->name()] = project;
            emit projectAdded(project);
        }
    }

    /**
     * @brief ProjectDatabase::removeProject
     * @param project
     */
    void ProjectDatabase::removeProject(const SharedProject &project)
    {
        if (!project)
            return;

        if (auto it = m_Projects.find(project->name()); it != m_Projects.end()) {
            m_Projects.erase(it);
            emit projectRemoved(project);
        }
    }

    /**
     * @brief ProjectDatabase::projectsAsVector
     * @return
     */
    ProjectsVector ProjectDatabase::projectsAsVector() const
    {
       return m_Projects.values().toVector();
    }

    /**
     * @brief ProjectDatabase::containsProject
     * @param name
     * @return
     */
    bool ProjectDatabase::contains(const QString &name) const
    {
        return m_Projects.contains(name) ;
    }

    /**
     * @brief ProjectDatabase::isEmpty
     * @return
     */
    bool ProjectDatabase::isEmpty() const
    {
       return m_Projects.isEmpty();
    }

    /**
     * @brief ProjectDatabase::projectByName
     * @param name
     * @return
     */
    SharedProject ProjectDatabase::projectByName(const QString &name) const noexcept
    {
        if (auto it = m_Projects.find(name); it != m_Projects.end())
            return *it;

        return nullptr;
    }

} // namespace Project
