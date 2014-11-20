/*****************************************************************************
**
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 19/11/2014.
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

#include "projectadaptor.h"

#include <QDebug>

#include <project/project.h>

namespace qml_adaptors {

    /**
     * @brief ProjectAdaptor::ProjectAdaptor
     * @param parent
     */
    ProjectAdaptor::ProjectAdaptor(QObject *parent)
        : QObject(parent)
    {
    }

    /**
     * @brief ProjectAdaptor::ProjectAdaptor
     * @param project
     * @param parent
     */
    ProjectAdaptor::ProjectAdaptor(const project::SharedProject &project, QObject *parent)
        : QObject(parent)
        , m_Project(project)
    {
    }

    /**
     * @brief ProjectAdaptor::ProjectAdaptor
     * @param src
     */
    ProjectAdaptor::ProjectAdaptor(const ProjectAdaptor &src)
        : QObject()
        , m_Project(src.project())
    {
    }

    /**
     * @brief ProjectAdaptor::~ProjectAdaptor
     */
    ProjectAdaptor::~ProjectAdaptor()
    {
    }

    /**
     * @brief ProjectAdaptor::project
     * @return
     */
    project::SharedProject ProjectAdaptor::project() const
    {
        return m_Project;
    }

    /**
     * @brief ProjectAdaptor::setProject
     * @param project
     */
    void ProjectAdaptor::setProject(const project::SharedProject &project)
    {
        m_Project = project;
    }

    /**
     * @brief ProjectAdaptor::name
     * @return
     */
    QString ProjectAdaptor::name() const
    {
        return m_Project->name();
    }

    /**
     * @brief ProjectAdaptor::id
     * @return
     */
    QString ProjectAdaptor::id() const
    {
        return m_Project->id();
    }

    /**
     * @brief ProjectAdaptor::path
     * @return
     */
    QString ProjectAdaptor::path() const
    {
        return m_Project->path();
    }

    /**
     * @brief ProjectAdaptor::toJson
     * @return
     */
    QJsonObject ProjectAdaptor::toJson() const
    {
        return m_Project->toJson();
    }

    /**
     * @brief ProjectAdaptor::fromJson
     * @param src
     * @param errorList
     */
    void ProjectAdaptor::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        m_Project->fromJson(src, errorList);
    }

    /**
     * @brief ProjectAdaptor::setName
     * @param name
     */
    void ProjectAdaptor::setName(const QString &name)
    {
        if (m_Project->name() == name)
            return;

        m_Project->setName(name);
        emit nameChanged(name);
    }

    /**
     * @brief ProjectAdaptor::setPath
     * @param path
     */
    void ProjectAdaptor::setPath(const QString &path)
    {
        if (m_Project->path() == path)
            return;

        m_Project->setPath(path);
        emit pathChanged(path);
    }

} // namespace qml_adaptors
