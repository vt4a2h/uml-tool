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
#include "ApplicationModel.h"

#include <Entity/Type.h>
#include <Entity/Class.h>
#include <Entity/ClassMethod.h>

#include "Constants.h"

namespace Models {

    /**
     * @brief ApplicationModal::ApplicationModal
     * @param parent
     */
    ApplicationModel::ApplicationModel()
        : QObject(nullptr)
        , m_GlobalDatabase(std::make_shared<DB::Database>())
        , m_TreeModel(std::make_shared<ProjectTreeModel>())
    {
        G_CONNECT(this, &ApplicationModel::currentProjectChanged,
                  m_TreeModel.get(), &ProjectTreeModel::onCurrentProjectChanged);
    }

    /**
     * @brief ApplicationModal::~ApplicationModal
     */
    ApplicationModel::~ApplicationModel() = default;

    /**
     * @brief ApplicationModel::makeProject
     * @return
     */
    Projects::SharedProject ApplicationModel::makeProject()
    {
        auto project = makeProject(DEFAULT_PROJECT_NAME, DEFAULT_PROJECT_PATH);
        return project;
    }

    /**
     * @brief ApplicationModal::makeProject
     * @param name
     * @param path
     * @return
     */
    Projects::SharedProject ApplicationModel::makeProject(const QString &name, const QString &path)
    {
        auto newProject(std::make_shared<Projects::Project>(name, path));
        newProject->setGlobalDatabase(globalDatabase());
        m_TreeModel->addProject(newProject);

        return *m_Projects.insert(newProject->name(), newProject);
    }

    /**
     * @brief ApplicationModel::addProject
     * @param pr
     * @return
     */
    bool ApplicationModel::addProject(const Projects::SharedProject &pr)
    {
        if (!pr || m_Projects.contains(pr->name()))
            return false;

        m_Projects[pr->name()] = pr;
        pr->setGlobalDatabase(globalDatabase());
        m_TreeModel->addProject(pr);
        return true;
    }

    /**
     * @brief ApplicationModel::getProject
     * @param id
     * @return
     */
    Projects::SharedProject ApplicationModel::project(const QString &name) const
    {
        return m_Projects[name];
    }

    /**
     * @brief ApplicationModal::projects
     * @return
     */
    Projects::ProjectsList ApplicationModel::projects() const
    {
        return m_Projects.values().toVector();
    }

    /**
     * @brief ApplicationModal::removeProject
     * @param id
     */
    bool ApplicationModel::removeProject(const QString &name)
    {
        auto it = m_Projects.find(name);
        if (it != m_Projects.end()) {
            m_TreeModel->removeProject(*it);
            return !!m_Projects.remove(name);
        }

        return false;
    }

    /**
     * @brief ApplicationModal::containsProject
     * @param id
     * @return
     */
    bool ApplicationModel::containsProject(const QString &name)
    {
        return m_Projects.contains(name);
    }

    /**
     * @brief ApplicationModel::addExistsType
     * @param type
     */
    void ApplicationModel::addExistsType(const QString &projectName, const Common::ID &scopeID,
                                         const Entity::SharedType &type)
    {
        if (auto &&pr = project(projectName))
            if (auto &&db = pr->database())
                if (auto &&scope = db->scope(scopeID))
                    scope->addExistsType(type);

        m_TreeModel->addType(type, scopeID, projectName);
    }

    /**
     * @brief ApplicationModel::removeType
     * @param projectID
     * @param scopeID
     * @param typeID
     */
    void ApplicationModel::removeType(const QString &projectName, const Common::ID &scopeID,
                                      const Common::ID &typeID)
    {
        if (auto &&pr = project(projectName))
            if (auto &&db = pr->database())
                if (auto &&scope = db->scope(scopeID))
                    scope->removeType(typeID);

        m_TreeModel->removeType(projectName, scopeID, typeID);
    }

    /**
     * @brief ApplicationModal::currentProject
     * @return
     */
    Projects::SharedProject ApplicationModel::currentProject() const
    {
        return m_CurrentProject;
    }

    /**
     * @brief ApplicationModal::setCurrentProject
     * @param id
     * @return
     */
    bool ApplicationModel::setCurrentProject(const QString &name)
    {
        if (name.isEmpty()) {
            auto previous = m_CurrentProject;
            if (previous)
                previous->database()->setClearGraphics(true);

            m_CurrentProject.reset();

            emit currentProjectChanged(previous, nullptr);
            return true;
        }

        auto it = m_Projects.find(name);
        if (it == m_Projects.end())
            return false;

        if (*it != m_CurrentProject) {
            auto previous = m_CurrentProject;
            if (previous)
                previous->database()->setClearGraphics(true);

            m_CurrentProject = *it;
            if (m_CurrentProject)
                m_CurrentProject->database()->setClearGraphics(false);

            emit currentProjectChanged(previous, m_CurrentProject);
            return true;
        }

        return false;
    }

    /**
     * @brief ApplicationModel::globalDatabase
     * @return
     */
    DB::SharedDatabase ApplicationModel::globalDatabase() const
    {
        return m_GlobalDatabase;
    }

    /**
     * @brief Aux method for tests
     * @param db
     */
    void ApplicationModel::setGlobalDatabse(const DB::SharedDatabase &db)
    {
        m_GlobalDatabase = db;
    }

    /**
     * @brief ApplicationModal::treeModel
     * @return
     */
    SharedTreeModel ApplicationModel::treeModel() const
    {
        return m_TreeModel;
    }

} // namespace models
