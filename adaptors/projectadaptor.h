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

#include "types.h"

namespace qml_adaptors {

    class ProjectAdaptor : public QObject
    {
        Q_OBJECT

    public:
        explicit ProjectAdaptor(QObject *parent = 0);
        explicit ProjectAdaptor(const project::SharedProject &project, QObject *parent = 0);
        ProjectAdaptor(const ProjectAdaptor &src);
        ~ProjectAdaptor();

        project::SharedProject project() const;
        void setProject(const project::SharedProject &project);

    signals:

    public slots:
        QString name() const; // just test

    private:
        project::SharedProject m_Project;
    };

} // namespace qml_adaptors

Q_DECLARE_METATYPE(qml_adaptors::ProjectAdaptor)
