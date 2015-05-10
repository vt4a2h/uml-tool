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

#include <db/db_types.hpp>

#include "types.h"

class QUndoStack;

/**
 * @brief project
 */
namespace project {

    /**
     * @brief The Project class
     */
    class Project : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
        Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
        Q_PROPERTY(QString id   READ id)

    public:
        explicit Project(QObject *parent = 0);
        Project(const QString &name, const QString &path, QObject *parent = 0);
        Project(const Project &src);
        ~Project();

        friend bool operator ==(const Project &lhs, const Project &rhs);

        QString name() const;
        QString path() const;
        QString id() const;

        void load(const QString &path); // don't forget install global database after load

        db::SharedProjectDatabase database() const;
        void setDatabase(const db::SharedProjectDatabase &database);

        db::SharedDatabase globalDatabase() const;
        bool setGloablDatabase(const db::SharedDatabase &database);

        QJsonObject toJson() const;
        void fromJson(const QJsonObject &src, QStringList &errorList);

        bool isSaved() const;

        bool hasErrors() const;
        ErrorList lastErrors() const;

        QUndoStack * commandsStack();
        const QUndoStack * commandsStack() const;

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
        QString databasePath(const QString &basePath) const;

        QString m_Name;
        QString m_Path;
        QString m_ID;

        bool m_SaveStatus;

        db::SharedProjectDatabase m_Database;

        ErrorList m_Errors;

        std::unique_ptr<QUndoStack> m_CommandsStack;
    };

} // namespace project

Q_DECLARE_METATYPE(project::Project)
