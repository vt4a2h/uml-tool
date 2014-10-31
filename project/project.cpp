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
    Project::Project()
        : Project( DEFAULT_PROJECT_NAME, DEFAULT_PROJECT_PATH, nullptr )
    {
    }

    /**
     * @brief Project::Project
     * @param name
     * @param path
     */
    Project::Project(const QString &name, const QString &path, const SharedErrorList &errors)
        : m_Name(name)
        , m_Path(path)
        , m_ID(utility::genId())
        , m_Database(std::make_shared<db::ProjectDatabase>())
        , m_Errors(errors)
    {
    }

    /**
     * @brief Project::Name
     * @return
     */
    QString Project::name() const
    {
        return m_Name;
    }

    /**
     * @brief Project::setName
     * @param name
     */
    void Project::setName(const QString &name)
    {
        m_Name = name;
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
     * @brief Project::setPath
     * @param path
     */
    void Project::setPath(const QString &path)
    {
        m_Path = path;
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
     * @brief Project::load
     * @param path
     */
    void Project::load(const QString &path)
    {
        Q_ASSERT(!!m_Errors);
        Q_ASSERT(!!m_Database);

        if (!utility::readFromFile(*this, projectPath(path)))
            *m_Errors << "Cannot read project file.";

        m_Database->setName(databaseFileName());
        m_Database->setPath(databasePath(path));
        m_Database->load(*m_Errors);
    }

    /**
     * @brief Project::save
     */
    void Project::save()
    {
        Q_ASSERT(!!m_Errors);
        Q_ASSERT(!!m_Database);

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
            if (!m_Database->save())
                *m_Errors << "Cannot save database to file.";
        } else {
            *m_Errors << "Project path is empty.";
        }
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
     * @brief Project::anyErrors
     * @return
     */
    bool Project::anyErrors() const
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
        result.insert("ID", m_ID);

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
