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

#include <types.h>

namespace models {

    /// The ApplicationModal class
    class ApplicationModel : public QObject
    {
        Q_OBJECT
    public:
        explicit ApplicationModel(QObject *parent = 0);
        ~ApplicationModel();

        project::SharedProject makeProject(const QString &name, const QString &path);
        project::SharedProject getProject(const QString &id) const;
        project::ProjectsList projects() const;
        bool removeProject(const QString &id);
        bool containsProject(const QString &id);

        project::SharedProject currentProject() const;
        bool setCurrentProject(const QString &id);

        SharedTreeModel treeModel() const;

    signals:
        void projectAdded(const project::SharedProject &pr);

    private:
        void connectTreeModel();

        project::Projects m_Projects;
        project::SharedProject m_CurrentProject;

        db::SharedDatabase m_GlobalDatabase;

        SharedTreeModel m_TreeModel;
    };

} // namespace models
