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

#include <project/project.h>
#include <db/projectdatabase.h>
#include <entity/scope.h>

#include "projecttreemodel.h"

#include "types.h"

namespace models {

    // TODO: handle projects ID changing

    /// The ApplicationModal class
    class ApplicationModel : public QObject
    {
        Q_OBJECT

    public:
        explicit ApplicationModel(QObject *parent = 0);
        ~ApplicationModel();

        // TODO: move to some database class
        project::SharedProject makeProject();
        project::SharedProject makeProject(const QString &name, const QString &path);
        bool addProject(const project::SharedProject &pr);
        project::SharedProject project(const entity::EntityID &id) const;
        project::ProjectsList projects() const;
        bool removeProject(const entity::EntityID &id);
        bool containsProject(const entity::EntityID &id);
        // }

        // TODO: remove from this class (breaks SRP) {
        entity::SharedScope makeScope(const QString &name);
        void addExistsScope(const entity::SharedScope &scope);
        void removeScope(const entity::EntityID &id);

        template <class T = entity::Type>
        std::shared_ptr<T> makeType(const entity::EntityID &scopeID, const QString &name = "");
        void addExistsType(const entity::EntityID &projectID, const entity::EntityID &scopeID, const entity::SharedType &type);
        void removeType(const entity::EntityID &projectID, const entity::EntityID &scopeID, const entity::EntityID &typeID);
        // }

        project::SharedProject currentProject() const;
        bool setCurrentProject(const entity::EntityID &id);

        db::SharedDatabase globalDatabase() const;

        SharedTreeModel treeModel() const;

    signals:
        void scopeAdded(const entity::SharedScope &scope);
        void currentProjectChanged(const project::SharedProject &p);

    private:
        project::Projects m_Projects;
        project::SharedProject m_CurrentProject;

        db::SharedDatabase m_GlobalDatabase;

        SharedTreeModel m_TreeModel;
    };

    template <class T>
    std::shared_ptr<T> ApplicationModel::makeType(const entity::EntityID &scopeID, const QString &name)
    {
        if (currentProject() && currentProject()->database()) {
            if (auto scope = currentProject()->database()->getScope(scopeID)) {
                if (auto type = scope->addType<T>(name)) {
                    m_TreeModel->addType(type, scope->id(), currentProject()->id());
                    connect(type.get(), &entity::BasicEntity::nameChanged,
                            m_CurrentProject.get(), &project::Project::touch);
                    // todo: connect scope id change to project touch
                    // note: moce to the factory class?

                    return type;
                }
            }
        }

        return std::shared_ptr<T>(); // stub
    }

} // namespace models
