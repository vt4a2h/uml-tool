#include "project.h"

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
     */
    void Project::load()
    {

    }

    /**
     * @brief Project::save
     */
    void Project::save()
    {

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


} // namespace project
