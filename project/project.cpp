#include "project.h"

#include <db/projectdatabase.h>
#include <utility/helpfunctions.h>

#include "constants.cpp"

namespace project {

    /**
     * @brief Project::Project
     */
    Project::Project()
        : Project( DEFAULT_PROJECT_NAME, DEFAULT_PROJECT_PATH )
    {
    }

    /**
     * @brief Project::Project
     * @param name
     * @param path
     */
    Project::Project(const QString &name, const QString &path)
        : m_Name(name)
        , m_Path(path)
        , m_ID(utility::genId())
        , m_Database(std::make_shared<db::ProjectDatabase>())
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

} // namespace project
