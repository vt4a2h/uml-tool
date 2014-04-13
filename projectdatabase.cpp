#include "projectdatabase.h"

#include <QJsonObject>

namespace db {

    ProjectDatabase::ProjectDatabase(const QString &name, const QString &path)
        : Database(name, path)
    {
    }

    void ProjectDatabase::clear()
    {
        Database::clear();
    }

    QJsonObject ProjectDatabase::toJson() const
    {
        QJsonObject result(Database::toJson());

        return result;
    }

    void ProjectDatabase::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        Database::fromJson(src, errorList);
    }

} // namespace db
