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

namespace Project {

    ProjectDatabase::ProjectDatabase() = default;

    void ProjectDatabase::addProject(const Projects::SharedProject &project)
    {
        if (project && !m_Projects.contains(project->name())) {
            m_Projects[project->name()] = project;
            emit projectAdded(project);
        }
    }

    void ProjectDatabase::removeProject(const Projects::SharedProject &project)
    {
        if (!project)
            return;

        if (auto it = m_Projects.find(project->name()); it != m_Projects.end()) {
            m_Projects.erase(it);
            emit projectRemoved(project);
        }
    }

} // namespace Project
