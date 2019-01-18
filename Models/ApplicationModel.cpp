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

#include <Project/ProjectDB.hpp>

#include "Constants.h"

namespace Models {

    /**
     * @brief ApplicationModal::ApplicationModal
     * @param parent
     */
    ApplicationModel::ApplicationModel()
        : QObject(nullptr)
        , m_projectsDb(std::make_unique<Projects::ProjectDatabase>())
        , m_GlobalDatabase(std::make_shared<DB::Database>())
        , m_TreeModel(std::make_shared<ProjectTreeModel>())
    {
        G_CONNECT(this, &ApplicationModel::currentProjectChanged,
                  m_TreeModel.get(), &ProjectTreeModel::onCurrentProjectChanged);
    }


    /**
     * @brief ApplicationModel::~ApplicationModel
     */
    ApplicationModel::~ApplicationModel() = default;

    /**
     * @brief ApplicationModel::makeProject
     * @return
     */
    Projects::SharedProject ApplicationModel::makeProject()
    {
        return makeProject(DEFAULT_PROJECT_NAME, DEFAULT_PROJECT_PATH);
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
        projectsDb().addProject(newProject);

        return newProject;
    }

    /**
     * @brief ApplicationModel::addProject
     * @param pr
     * @return
     */
    bool ApplicationModel::addProject(const Projects::SharedProject &pr)
    {
        if (!pr || projectsDb().contains(pr->name()))
            return false;

        projectsDb().addProject(pr);
        pr->setGlobalDatabase(globalDatabase());
        m_TreeModel->addProject(pr);
        return true;
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

        if (auto proj = projectsDb().projectByName(name); proj != m_CurrentProject) {
            auto previous = m_CurrentProject;
            if (previous)
                previous->database()->setClearGraphics(true);

            m_CurrentProject = proj;
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

    /**
     * @brief ApplicationModel::projectsDb
     * @return
     */
    Projects::ProjectDatabase &ApplicationModel::projectsDb()
    {
        return *m_projectsDb;
    }

    /**
     * @brief ApplicationModel::projectsDb
     * @return
     */
    const Projects::ProjectDatabase &ApplicationModel::projectsDb() const
    {
        return *m_projectsDb;
    }

} // namespace models
