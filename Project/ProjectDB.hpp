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
#pragma once

#include <Project/ProjectTypes.hpp>

namespace Projects {

    class ProjectDatabase : public QObject
    {
        Q_OBJECT

    public:
        ProjectDatabase();

        void addProject(const SharedProject &project);
        void removeProject(const SharedProject &project);

        ProjectsVector projectsAsVector() const;

        bool contains(const QString &name) const;
        bool isEmpty() const;

        SharedProject projectByName(const QString &name) const noexcept;

    signals:
        void projectAdded(const SharedProject &project);
        void projectRemoved(const SharedProject &project);

    private:
        Projects m_Projects;
    };

} // namespace Projects
