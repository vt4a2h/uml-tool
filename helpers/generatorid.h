/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 17/01/2016.
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

#include <QtCore/QObject>

#include <project/project_types.hpp>

#include <entity/entityid.h>

namespace entity {

    /// Generator ID for current project items (entities, relations, bases etc.)
    class GeneratorID : public QObject
    {
        Q_OBJECT

    public:
        GeneratorID(const GeneratorID &) = delete;
        GeneratorID(GeneratorID &&) = delete;
        GeneratorID& operator =(const GeneratorID&) = delete;
        GeneratorID& operator =(GeneratorID&&) = delete;

        static const GeneratorID &instance();

        entity::EntityID genID() const;

    public slots:
        void onCurrentProjectChanged(const project::SharedProject &p);

    private:
        explicit GeneratorID(QObject *parent = 0);
        project::SharedProject project() const;

        project::WeakProject m_CurrentProject;
    };

} // entity
