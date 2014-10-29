#include "project.h"

#include <db/projectdatabase.h>
#include <utility/helpfunctions.h>

#include "constants.cpp"

namespace project {

    Project::Project()
        : Project( DEFAULT_PROJECT_NAME, DEFAULT_PROJECT_PATH )
    {
    }

    Project::Project(const QString &name, const QString &path)
        : m_Name(name)
        , m_Path(path)
        , m_ID(utility::genId())
        , m_Database(std::make_shared<db::ProjectDatabase>())
    {
    }

    QString Project::Name() const
    {
        return m_Name;
    }

    void Project::setName(const QString &name)
    {
        m_Name = name;
    }

    QString Project::path() const
    {
        return m_Path;
    }

    void Project::setPath(const QString &path)
    {
        m_Path = path;
    }

    QString Project::id() const
    {
        return m_ID;
    }

    void Project::load()
    {

    }

    void Project::save()
    {

    }

} // namespace project
