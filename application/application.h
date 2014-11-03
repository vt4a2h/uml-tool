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

#include "types.h"

/**
 * @brief namespace application
 */
namespace application {

    /**
     * @brief The Application class
     */
    class Application
    {
    public:
        Application(const Application &) = delete;
        Application(Application &&) = delete;

        Application& operator= (const Application &)  = delete;
        Application& operator= (Application &&)  = delete;

        void init();

        bool hasErrors() const;
        SharedErrorList errors() const;

        project::SharedProject createProject(const QString &name, const QString &path);
        project::SharedProject findProjectById(const QString &id) const;
        project::SharedProject findProjectByName(const QString &name) const;
        project::ProjectsList projects() const;
        uint projectsCount() const;
        void removeProjectById(const QString &id);
        void removeProjectByName(const QString &name);

        static Application& instance()
        {
            static Application singleInstance;
            return singleInstance;
        }

    private:
        Application();
        void readConfig();

        project::Projects m_Projects;
        SharedErrorList m_ErrorList;
    };

} // namespace application
