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
#include <QJsonObject>

#include <Common/ID.h>
#include <Common/SharedFromThis.h>

#include <Commands/CommandsTypes.h>

#include <DB/DBTypes.hpp>

#include <Entity/EntityTypes.hpp>

#include <GUI/graphics/GraphicsTypes.h>

#include "types.h"
#include "ProjectTypes.hpp"

class QUndoStack;

/**
 * @brief project
 */
namespace Projects {

    /**
     * @brief The Project class
     */
    class Project : public QObject, public Common::SharedFromThis<Project>
    {
        Q_OBJECT
        Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
        Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)

    public:
        Q_DISABLE_COPY(Project)

        explicit Project();
        Project(const QString &name, const QString &path);
        Project(Project &&src) noexcept;
        ~Project();

        Project& operator =(Project &&lhs) noexcept;
        friend bool operator ==(const Project &lhs, const Project &rhs);

        QString name() const;
        QString path() const;

        QString fullPath() const;

        void load(const QString &path); // don't forget install global database after load

        DB::SharedProjectDatabase database() const;
        void setDatabase(const DB::SharedProjectDatabase &database);

        DB::SharedDatabase globalDatabase() const;
        bool setGlobalDatabase(const DB::SharedDatabase &database);

        QJsonObject toJson() const;
        void fromJson(const QJsonObject &src, QStringList &errorList);

        bool isSaved() const;

        bool hasErrors() const;
        ErrorList lastErrors() const;

        Common::ID genID();

    public slots:
        void setSaveStatus(bool newStatus);
        void setName(const QString &name);
        void setPath(const QString &path);

        void save();
        void touch();

    signals:
        void nameChanged(const QString &name);
        void pathChanged(const QString &path);

        void errors(const QString &message, const ErrorList &errorsList);

        void saved();
        void modified();

    private:
        QString projectFileName() const;
        QString databaseFileName() const;
        QString projectPath(const QString &basePath) const;
        void makeConnections();

        QString m_Name;
        QString m_Path;

        Common::ID m_nextUniqueID;

        bool m_SaveStatus;

        DB::SharedProjectDatabase m_Database;

        ErrorList m_Errors;

        Commands::SharedCommandStack m_CommandsStack;
    };

    /// Helper for project load. Should be in the header due to Qt Meta system limitation
    class ScopedProjectSetter : public QObject
    {
        Q_OBJECT

    public:
        ScopedProjectSetter(const SharedProject &p);
        ~ScopedProjectSetter();

    signals:
        void projectChanged(const SharedProject &p, const SharedProject &c);

    private:
        SharedProject m_OldProject;
    };

    uint qHash(const SharedProject &p);

} // namespace project
