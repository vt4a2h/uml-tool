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

#include <project/project.h>
#include <db/projectdatabase.h>
#include <db/database.h>
#include <entity/scope.h>
#include <entity/type.h>
#include <entity/class.h>
#include <entity/classmethod.h>

#include "projecttreemodel.h"

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
//        // make some test data
//        makeProject("Project1", "/path1");
//        makeProject("Project2", "/path2");

//        project::SharedProject pr3 = makeProject("Project3", "/path3");
//        db::SharedProjectDatabase db3 = pr3->database();
//        db3->addScope("foo_scope");
//        db3->addScope("bar_scope");
//        entity::SharedScope sc1 = db3->addScope("baz_scope");
//        sc1->addType("foo_type");
//        sc1->addType("bar_type");
//        entity::SharedClass cl1 = sc1->addType<entity::Class>("baz_type");
//        cl1->addField("foo_field", cl1->id(), "m_");
//        cl1->makeMethod("bar_method");

//        // duplication of projects... but still useful for tests
//        for (auto &&pr : m_Projects)
//            m_TreeModel->addProject(pr);
    }

    /**
     * @brief ApplicationModal::~ApplicationModal
     */
    ApplicationModel::~ApplicationModel()
    {
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
        m_TreeModel->addProject(newProject);
        return *m_Projects.insert(newProject->id(), newProject);
    }

    /**
     * @brief ApplicationModel::getProject
     * @param id
     * @return
     */
    project::SharedProject ApplicationModel::getProject(const QString &id) const
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
     * @brief ApplicationModal::treeModel
     * @return
     */
    SharedTreeModel ApplicationModel::treeModel() const
    {
        return m_TreeModel;
    }

} // namespace models
