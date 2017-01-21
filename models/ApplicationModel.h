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

#include <project/Project.h>
#include <db/ProjectDatabase.h>
#include <entity/Scope.h>

#include "ProjectTreeModel.h"

#include "types.h"

namespace models {

    // TODO: handle projects ID changing

    /// The ApplicationModal class
    class ApplicationModel : public QObject
    {
        Q_OBJECT

    public:
        explicit ApplicationModel();
        ~ApplicationModel();

        // TODO: move to some database class
        project::SharedProject makeProject();
        project::SharedProject makeProject(const QString &name, const QString &path);
        bool addProject(const project::SharedProject &pr);
        project::SharedProject project(const QString &name) const;
        project::ProjectsList projects() const;
        bool removeProject(const QString &name);
        bool containsProject(const QString& name);
        // }

        // TODO: remove from this class (breaks SRP) {
        entity::SharedScope makeScope(const QString &name);
        void addExistsScope(const entity::SharedScope &scope);
        void removeScope(const common::ID &id);

        void addExistsType(const QString &projectName, const common::ID &scopeID, const entity::SharedType &type);
        void removeType(const QString &projectName, const common::ID &scopeID, const common::ID &typeID);
        // }

        project::SharedProject currentProject() const;
        bool setCurrentProject(const QString &name);

        db::SharedDatabase globalDatabase() const;
        void setGlobalDatabse(const db::SharedDatabase &db);

        SharedTreeModel treeModel() const;

    signals:
        void scopeAdded(const entity::SharedScope &scope);
        void currentProjectChanged(const project::SharedProject &previous,
                                   const project::SharedProject &current);

    private:
        project::Projects m_Projects;
        project::SharedProject m_CurrentProject;

        db::SharedDatabase m_GlobalDatabase;

        SharedTreeModel m_TreeModel;
    };

} // namespace models
