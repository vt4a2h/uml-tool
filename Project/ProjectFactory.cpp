/*****************************************************************************
**
** Copyright (C) 2019 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 6/1/2019.
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
#include "ProjectFactory.hpp"

#include "Constants.h"
#include "Project.h"

namespace Projects {

    ProjectFactory::~ProjectFactory() = default;
    ProjectFactory::ProjectFactory() = default;

    ProjectFactory &ProjectFactory::instance()
    {
        static ProjectFactory factory;
        return factory;
    }

    void ProjectFactory::initialise(DB::SharedDatabase globalDb)
    {
        m_GlobalDB = std::move(globalDb);
    }

    DB::SharedDatabase ProjectFactory::globalDB() const
    {
        return m_GlobalDB;
    }

    SharedProject ProjectFactory::makeProject() const
    {
        return makeProject(DEFAULT_PROJECT_NAME, DEFAULT_PROJECT_PATH);
    }

    SharedProject ProjectFactory::makeProject(const QString &name, const QString &path) const
    {
        auto project = std::make_shared<Project>(name, path);

        Q_ASSERT(!!globalDB());
        project->setGlobalDatabase(globalDB());

        return project;
    }

} // namespace Projects
