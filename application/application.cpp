/*****************************************************************************
** 
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 03/11/2014.
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

#include "application.h"

#include <functional>

#include <QFileInfo>
#include <QDir>

#include <project/project.h>

#include "constants.cpp"

namespace application {

    /**
     * @brief Application::init
     */
    void Application::init()
    {
        readConfig();
    }

    /**
     * @brief Application::hasErrors
     * @return
     */
    bool Application::hasErrors() const
    {
        return !m_ErrorList->isEmpty();
    }

    /**
     * @brief Application::errors
     * @return
     */
    SharedErrorList Application::errors() const
    {
        return m_ErrorList;
    }

    /**
     * @brief Application::createProject
     * @param name
     * @param path
     * @return
     */
    project::SharedProject Application::createProject(const QString &name, const QString &path)
    {
        auto newProject = std::make_shared<project::Project>(name, path, m_ErrorList);
        return *m_Projects.insert(newProject->id(), newProject);
    }

    /**
     * @brief Application::findProjectById
     * @param id
     * @return
     */
    project::SharedProject Application::findProjectById(const QString &id) const
    {
        return m_Projects.value(id, nullptr);
    }

    /**
     * @brief Application::findProjectByName
     * @param name
     * @return
     */
    project::SharedProject Application::findProjectByName(const QString &name) const
    {
        auto projects = m_Projects.values();
        auto it = std::find_if(projects.begin(), projects.end(),
                               [&](const project::SharedProject &p){ return p->name() == name; });
        return it != projects.end() ? *it : nullptr;
    }

    /**
     * @brief Application::projects
     * @return
     */
    project::ProjectsList Application::projects() const
    {
        return m_Projects.values();
    }

    /**
     * @brief Application::projectsCount
     * @return
     */
    uint Application::projectsCount() const
    {
        return m_Projects.count();
    }

    /**
     * @brief Application::removeProjectById
     * @param id
     */
    void Application::removeProjectById(const QString &id)
    {
        m_Projects.remove(id);
    }

    /**
     * @brief Application::removeProjectByName
     * @param name
     */
    void Application::removeProjectByName(const QString &name)
    {
        auto project = findProjectByName(name);
        if (project)
            m_Projects.remove(project->id());
    }

    /**
     * @brief Application::Application
     */
    Application::Application()
        : m_ErrorList(std::make_shared<ErrorList>())
    {
    }

    /**
     * @brief Application::readConfig
     */
    void Application::readConfig()
    {
        QString filePath(QDir::toNativeSeparators(QDir::currentPath() + "/" + CFG_FILE_NAME));

        if (QFileInfo(filePath).exists()) {
            // read config
        } else {
            *m_ErrorList << "Configuration file is not found.";
        }
    }

} // namespace application
