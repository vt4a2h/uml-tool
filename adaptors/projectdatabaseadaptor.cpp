/*****************************************************************************
**
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 20/11/2014.
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

#include "projectdatabaseadaptor.h"

namespace qml_adaptors {

    /**
     * @brief ProjectDatabaseAdaptor::ProjectDatabaseAdaptor
     * @param parent
     */
    ProjectDatabaseAdaptor::ProjectDatabaseAdaptor(QObject *parent)
        : ProjectDatabaseAdaptor(nullptr, parent)
    {
    }

    /**
     * @brief ProjectDatabaseAdaptor::ProjectDatabaseAdaptor
     * @param db
     * @param parent
     */
    ProjectDatabaseAdaptor::ProjectDatabaseAdaptor(const db::SharedProjectDatabase &db, QObject *parent)
        : QObject(parent)
        , m_db(db)
    {
    }

    /**
     * @brief ProjectDatabaseAdaptor::ProjectDatabaseAdaptor
     * @param src
     */
    ProjectDatabaseAdaptor::ProjectDatabaseAdaptor(const ProjectDatabaseAdaptor &src)
        : QObject(src.parent())
        , m_db(src.m_db)
    {
    }

    /**
     * @brief ProjectDatabaseAdaptor::~ProjectDatabaseAdaptor
     */
    ProjectDatabaseAdaptor::~ProjectDatabaseAdaptor()
    {

    }

} // namespace qml_adaptors
