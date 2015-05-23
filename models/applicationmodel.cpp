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
#include "applicationmodel.h"

#include <entity/type.h>
#include <entity/class.h>
#include <entity/classmethod.h>

#include "constants.cpp"

namespace models {

    /**
     * @brief ApplicationModal::ApplicationModal
     * @param parent
     */
    ApplicationModel::ApplicationModel(QObject *parent)
        : QObject(parent)
        , m_GlobalDatabase(std::make_shared<db::Database>())
        , m_TreeModel(std::make_shared<ProjectTreeModel>())
    {
    }

    /**
     * @brief ApplicationModal::~ApplicationModal
     */
    ApplicationModel::~ApplicationModel()
    {
    }

    /**
     * @brief ApplicationModel::makeProject
     * @return
     */
    project::SharedProject ApplicationModel::makeProject()
    {
        return makeProject(DEFAULT_PROJECT_NAME, DEFAULT_PROJECT_PATH);
    }

    /**
     * @brief ApplicationModal::makeProject
     * @param name
     * @param path
     * @return
     */
    project::SharedProject ApplicationModel::makeProject(const QString &name, const QString &path)
    {
        project::SharedProject newProject(std::make_shared<project::Project>(name, path));
        newProject->setGloablDatabase(globalDatabase());
        m_TreeModel->addProject(newProject);
        return *m_Projects.insert(newProject->id(), newProject);
    }

    /**
     * @brief ApplicationModel::addProject
     * @param pr
     * @return
     */
    bool ApplicationModel::addProject(const project::SharedProject &pr)
    {
        if (!m_Projects.contains(pr->id())) {
            m_Projects[pr->id()] = pr;
            m_TreeModel->addProject(pr);
            return true;
        }

        return false;
    }

    /**
     * @brief ApplicationModel::getProject
     * @param id
     * @return
     */
    project::SharedProject ApplicationModel::project(const QString &id) const
    {
        return m_Projects[id];
    }

    /**
     * @brief ApplicationModal::projects
     * @return
     */
    project::ProjectsList ApplicationModel::projects() const
    {
        return m_Projects.values();
    }

    /**
     * @brief ApplicationModal::removeProject
     * @param id
     */
    bool ApplicationModel::removeProject(const QString &id)
    {
        return !!m_Projects.remove(id);
    }

    /**
     * @brief ApplicationModal::containsProject
     * @param id
     * @return
     */
    bool ApplicationModel::containsProject(const QString &id)
    {
        return m_Projects.contains(id);
    }

    /**
     * @brief ApplicationModel::makeScope
     * @param name
     * @return
     */
    entity::SharedScope ApplicationModel::makeScope(const QString &name)
    {
        entity::SharedScope result;

        if (m_CurrentProject) {
            auto projectDB = m_CurrentProject->database();
            Q_ASSERT(projectDB);
            result = projectDB->addScope(name);
            m_TreeModel->addScope(result, m_CurrentProject->id());
            // TODO: add scope with parent scope

            currentProject()->touch();
        }

        return result;
    }

    /**
     * @brief ApplicationModel::addExistsScope
     * @param scope
     */
    void ApplicationModel::addExistsScope(const entity::SharedScope &scope)
    {
        if (m_CurrentProject && scope) {
            auto projectDB = m_CurrentProject->database();
            Q_ASSERT(projectDB);
            projectDB->addExistsScope(scope);
            m_TreeModel->addScope(scope, m_CurrentProject->id());
        }
    }

    /**
     * @brief ApplicationModel::removeScope
     * @param id
     */
    void ApplicationModel::removeScope(const QString &id)
    {
        if (m_CurrentProject) {
            m_CurrentProject->database()->removeScope(id);
            m_TreeModel->removeScope(id, m_CurrentProject->id());
            m_CurrentProject->touch();
        }
    }

    /**
     * @brief ApplicationModel::addExistsType
     * @param type
     */
    void ApplicationModel::addExistsType(const QString &projectID, const QString &scopeID,
                                         const entity::SharedType &type)
    {
        if (auto &&pr = project(projectID))
            if (auto &&db = pr->database())
                if (auto &&scope = db->getScope(scopeID)) {
                    scope->addExistsType(type);
                    connect(type.get(), &entity::BasicEntity::nameChanged,
                            m_CurrentProject.get(), &project::Project::touch);
                    // todo: connect scope id change to project touch
                    // note: moce to the factory class?
                }

        m_TreeModel->addType(type, scopeID, projectID);
    }

    /**
     * @brief ApplicationModel::removeType
     * @param projectID
     * @param scopeID
     * @param typeID
     */
    void ApplicationModel::removeType(const QString &projectID, const QString &scopeID,
                                      const QString &typeID)
    {
        if (auto &&pr = project(projectID))
            if (auto &&db = pr->database())
                if (auto &&scope = db->getScope(scopeID))
                    scope->removeType(typeID);

        m_TreeModel->removeType(projectID, scopeID, typeID);
    }

    /**
     * @brief ApplicationModal::currentProject
     * @return
     */
    project::SharedProject ApplicationModel::currentProject() const
    {
        return m_CurrentProject;
    }

    /**
     * @brief ApplicationModal::setCurrentProject
     * @param id
     * @return
     */
    bool ApplicationModel::setCurrentProject(const QString &id)
    {
        if (!m_Projects.contains(id))
            return false;

        m_CurrentProject = m_Projects[id];
        return true;
    }

    /**
     * @brief ApplicationModel::globalDatabase
     * @return
     */
    db::SharedDatabase ApplicationModel::globalDatabase() const
    {
        return m_GlobalDatabase;
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
