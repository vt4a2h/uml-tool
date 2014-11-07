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

#include <QString>

#include "types.h"

/**
 * @brief project
 */
namespace project {

    /**
     * @brief The Project class
     */
    class Project
    {
    public:
        Project();
        Project(const QString &name, const QString &path, const SharedErrorList &errors);
        virtual ~Project() {}

        friend bool operator ==(const Project &lhs, const Project &rhs);

        QString name() const;
        void setName(const QString &name);

        QString path() const;
        void setPath(const QString &path);

        QString id() const;

        void load(const QString &path); // don't forget install global database after load
        void save();

        db::SharedProjectDatabase database() const;
        void setDatabase(const db::SharedProjectDatabase &database);

        db::SharedDatabase globalDatabase() const;
        bool setGloablDatabase(const db::SharedDatabase &database);

        bool hasErrors() const;
        SharedErrorList errors() const;
        void setErrorsList(const SharedErrorList &errors);

        virtual QJsonObject toJson() const;
        virtual void fromJson(const QJsonObject &src, QStringList &errorList);

    protected:
        virtual QString projectFileName() const;
        virtual QString databaseFileName() const;
        virtual QString projectPath(const QString &basePath) const;
        virtual QString databasePath(const QString &basePath) const;

        QString m_Name;
        QString m_Path;
        QString m_ID;

        db::SharedProjectDatabase m_Database;
        SharedErrorList m_Errors;
    };

} // namespace project
