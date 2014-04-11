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

        entity::SharedScope getScope(const QString &id) const;
        entity::SharedScope addScope(const QString &name = "", const QString &parentScopeId = "");
        bool containsScope(const QString &id) const;
        void removeScope(const QString &id);
        entity::ScopesList scopes();

        QStringList depthScopeFind(const QString &id) const;
        entity::SharedScope depthGetScope(const QStringList &ids) const;

        void load(QStringList &errorList);
        void clear();
        void save() const;

        QJsonObject toJson() const;
        void fromJson(const QJsonObject &src, QStringList &errorList);

    private:
        QString makeFullPath() const;
        void recursiveFind(entity::SharedScope scope, const QString &id, QStringList &ids) const;

        QString        m_Name;
        QString        m_Path;
        entity::Scopes m_Scopes;
    };

} // namespace db

