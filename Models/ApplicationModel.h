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
#pragma once

#include <QObject>

#include <Project/Project.h>
#include <DB/ProjectDatabase.h>
#include <Entity/Scope.h>

#include "ProjectTreeModel.h"

#include "types.h"

namespace Models {

    // TODO: handle projects ID changing

    /// The ApplicationModal class
    class ApplicationModel : public QObject
    {
        Q_OBJECT

    public:
        explicit ApplicationModel();

        ~ApplicationModel() override;

        // TODO: move to some database class
        Projects::SharedProject makeProject();
        Projects::SharedProject makeProject(const QString &name, const QString &path);
        bool addProject(const Projects::SharedProject &pr);
        // }

        Projects::SharedProject currentProject() const;
        bool setCurrentProject(const QString &name);

        DB::SharedDatabase globalDatabase() const;
        void setGlobalDatabse(const DB::SharedDatabase &db);

        SharedTreeModel treeModel() const;

        Projects::ProjectDatabase &projectsDb();
        const Projects::ProjectDatabase &projectsDb() const;

    signals:
        void currentProjectChanged(const Projects::SharedProject &previous,
                                   const Projects::SharedProject &current);

    private:
        Projects::UniqueProjectDb m_projectsDb;
        Projects::SharedProject m_CurrentProject;

        DB::SharedDatabase m_GlobalDatabase;

        SharedTreeModel m_TreeModel;
    };

} // namespace models
