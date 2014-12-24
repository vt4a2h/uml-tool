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

#include "project.h"

#include <QDir>
#include <QFile>
#include <QJsonObject>

#include <db/projectdatabase.h>
#include <utility/helpfunctions.h>

#include "constants.cpp"

namespace project {

    /**
     * @brief Project::Project
     */
    Project::Project(QObject *parent)
        : Project(DEFAULT_PROJECT_NAME, DEFAULT_PROJECT_PATH, nullptr, parent)
    {}

    /**
     * @brief Project::Project
     * @param name
     * @param path
     */
    Project::Project(const QString &name, const QString &path, const SharedErrorList &errors, QObject *parent)
        : QObject(parent)
        , m_Name(name)
        , m_Path(path)
        , m_ID(utility::genId())
        , m_SaveStatus(false)
        , m_Database(std::make_shared<db::ProjectDatabase>())
        , m_Errors(errors)
    {}

    /**
     * @brief Project::Project
     * @param src
     */
    Project::Project(const Project &src)
        : QObject(nullptr)
        , m_Name(src.name())
        , m_Path(src.path())
        , m_ID(utility::genId()) // new UUID
        , m_SaveStatus(false)    // not saved
         // deep copy of project database and shallow copy of global database
        , m_Database(std::make_shared<db::ProjectDatabase>(*src.m_Database))
        , m_Errors(src.errors())
    {}

    /**
     * @brief Project::~Project
     */
    Project::~Project()
    {}

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const Project &lhs, const Project &rhs)
    {
        Q_ASSERT(!!lhs.m_Errors);
        Q_ASSERT(!!lhs.m_Database);
        Q_ASSERT(!!rhs.m_Errors);
        Q_ASSERT(!!rhs.m_Database);

        return lhs.m_Name == rhs.m_Name &&
               lhs.m_Path == rhs.m_Path &&
               lhs.m_ID   == rhs.m_ID   &&
               *lhs.m_Database == *rhs.m_Database &&
               *lhs.m_Errors   == *rhs.m_Errors;
    }

    /**
     * @brief Project::load
     * @param path
     */
    void Project::load(const QString &path)
    {
        Q_ASSERT(!!m_Errors);
        Q_ASSERT(!!m_Database);

        m_Errors->clear();

        if (!utility::readFromFile(*this, path))
            *m_Errors << "Cannot read project file.";

        m_Database->setPath(databasePath(m_Path));
        if (!utility::readFromFile(*m_Database, m_Database->path()))
            *m_Errors << "Cannot read database file.";

        m_SaveStatus = m_Errors->isEmpty();
    }

    /**
     * @brief Project::save
     */
    void Project::save()
    {
        Q_ASSERT(!!m_Errors);
        Q_ASSERT(!!m_Database);

        m_Errors->clear();

        if (!m_Path.isEmpty()) {
            QDir dir(m_Path);
            if (!dir.exists())
                if (!dir.mkpath(m_Path)) {
                    *m_Errors << "Cannot create project directory.";
                    return;
                }

            if (!utility::writeToFile(*this, projectPath(m_Path)))
                *m_Errors << "Cannot save project to file.";

            m_Database->setName(databaseFileName());
            m_Database->setPath(databasePath(m_Path));
            if (!utility::writeToFile(*m_Database, m_Database->path()))
                *m_Errors << "Cannot save database to file.";
        } else {
            *m_Errors << "Project path is empty.";
        }

        m_SaveStatus = m_Errors->isEmpty();
    }

    /**
     * @brief Project::db
     * @return
     */
    db::SharedProjectDatabase Project::database() const
    {
        return m_Database;
    }

    /**
     * @brief Project::setDatabase
     * @param db
     */
    void Project::setDatabase(const db::SharedProjectDatabase &database)
    {
        m_Database = database;
    }

    /**
     * @brief Project::globalDatabase
     * @return
     */
    db::SharedDatabase Project::globalDatabase() const
    {
        return m_Database ? m_Database->globalDatabase() : nullptr;
    }

    /**
     * @brief Project::setGloablDatabase
     * @param database
     * @return
     */
    bool Project::setGloablDatabase(const db::SharedDatabase &database)
    {
        bool result(false);

        if (m_Database)
            m_Database->setGlobalDatabase(database);

        return result;
    }

    /**
     * @brief Project::hasErrors
     * @return
     */
    bool Project::hasErrors() const
    {
        return m_Errors ? !m_Errors->isEmpty() : false;
    }

    /**
     * @brief Project::errors
     * @return
     */
    SharedErrorList Project::errors() const
    {
        return m_Errors;
    }

    /**
     * @brief Project::setErrorsList
     * @param errors
     */
    void Project::setErrorsList(const SharedErrorList &errors)
    {
        m_Errors = errors;
    }

    /**
     * @brief Project::toJson
     * @return
     */
    QJsonObject Project::toJson() const
    {
        QJsonObject result;

        result.insert("Name", m_Name);
        result.insert("Path", m_Path);
        result.insert("ID"  , m_ID  );

        return result;
    }

    /**
     * @brief Project::fromJson
     * @param src
     * @param errorList
     */
    void Project::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        utility::checkAndSet(src, "Name", errorList, [&src, this](){
            m_Name = src["Name"].toString();
        });
        utility::checkAndSet(src, "Path", errorList, [&src, this](){
            m_Path = src["Path"].toString();
        });
        utility::checkAndSet(src, "ID", errorList, [&src, this](){
            m_ID = src["ID"].toString();
        });
    }

    /**
     * @brief Project::isSaved
     * @return
     */
    bool Project::isSaved() const
    {
        return m_SaveStatus;
    }

    /**
     * @brief Project::setSaveStatus
     * @param newStatus
     */
    void Project::setSaveStatus(bool newStatus)
    {
        m_SaveStatus = newStatus;
    }

    /**
     * @brief Project::name
     * @return
     */
    QString Project::name() const
    {
        return m_Name;
    }

    /**
     * @brief Project::path
     * @return
     */
    QString Project::path() const
    {
        return m_Path;
    }

    /**
     * @brief Project::id
     * @return
     */
    QString Project::id() const
    {
        return m_ID;
    }

    /**
     * @brief Project::setName
     * @param name
     */
    void Project::setName(const QString &name)
    {
        if (m_Name == name)
            return;

        m_Name = name;
        emit nameChanged(name);
    }

    /**
     * @brief Project::setPath
     * @param path
     */
    void Project::setPath(const QString &path)
    {
        if (m_Path == path)
            return;

        m_Path = path;
        emit pathChanged(path);
    }

    /**
     * @brief Project::projectFileName
     * @return
     */
    QString Project::projectFileName() const
    {
        return m_Name.toLower().replace(" ", "_");
    }

    /**
     * @brief Project::databaseFileName
     * @return
     */
    QString Project::databaseFileName() const
    {
        return m_Name.toLower().replace(" ", "_") + "_db";
    }

    /**
     * @brief Project::makeProjectPath
     * @param basePath
     * @return
     */
    QString Project::projectPath(const QString &basePath) const
    {
        QChar sep = QDir(basePath).separator();
        return basePath + sep + projectFileName() + "." + PROJECT_FILE_EXTENTION;
    }

    /**
     * @brief Project::makeDatabasePath
     * @param basePath
     * @return
     */
    QString Project::databasePath(const QString &basePath) const
    {
        QChar sep = QDir(basePath).separator();
        return basePath + sep + databaseFileName() + "." + DATABASE_FILE_EXTENTION;
    }

} // namespace project
