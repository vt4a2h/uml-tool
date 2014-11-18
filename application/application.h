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

#pragma once

#include <QObject>
#include <QQmlApplicationEngine>

#include "types.h"

/**
 * @brief namespace application
 */
namespace application {

    /**
     * @brief The Application class
     */
    class Application : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(bool anyErrors READ hasErrors)
        Q_PROPERTY(ErrorList errors READ getErrors)

    public:
        explicit Application(QObject *parent = nullptr);
        ~Application();

        void run();

        bool hasErrors() const;
        ErrorList getErrors() const;

        project::SharedProject findProjectById(const QString &id) const;
        project::SharedProject findProjectByName(const QString &name) const;
        project::ProjectsList projects() const;
        uint projectsCount() const;
        void removeProjectById(const QString &id);
        void removeProjectByName(const QString &name);

    public slots:
        void createProject(const QString &name, const QString &path);
        bool openProject(const QString &path);
        bool setActiveProject(const QString &id);

    signals:
        void projectCreated(const QJsonObject &project);
        void activeProjectChange(const QJsonObject &project);
        void errors(const QString &message, const ErrorList &errorlist);

    private:
        void init();
        void configuredGui();
        void readDatabase();
        void readConfig();

        db::SharedDatabase m_GlobalDatabase;

        project::Projects m_Projects;
        project::SharedProject m_ActivProject;
        SharedErrorList m_ErrorList;

        QQmlApplicationEngine m_Engine;
    };

} // namespace application

Q_DECLARE_METATYPE(ErrorList)
Q_DECLARE_METATYPE(project::SharedProject)
