#pragma once

#include "types.h"

namespace db {

    class Database
    {
    public:
        Database(const QString &name = "", const QString &path = "");

        QString path() const;
        void setPath(const QString &path);

        QString name() const;
        void setName(const QString &name);

        void load(QStringList &errorList);
        void clear();
        void save() const;

        QJsonObject toJson() const;
        void fromJson(const QJsonObject &src, QStringList &errorList);

    private:
        QString makeFullPath() const;

        QString        m_Name;
        QString        m_Path;
        entity::Scopes m_Scopes;
    };

} // namespace db

