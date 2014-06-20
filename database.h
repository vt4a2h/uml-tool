#pragma once

#include "types.h"

namespace db {

    class Database
    {
    public:
        Database(const QString &name = "", const QString &path = "");
        virtual ~Database();

        QString path() const;
        void setPath(const QString &path);

        QString name() const;
        void setName(const QString &name);

        entity::SharedScope getScope(const QString &id) const;
        entity::SharedScope addScope(const QString &name = "", const QString &parentScopeId = "");
        bool containsScope(const QString &id) const;
        bool anyScopes() const;
        void removeScope(const QString &id);
        entity::ScopesList scopes();

        entity::SharedScope depthScopeSearch(const QString &scopeId) const;
        entity::SharedType  depthTypeSearch(const QString &typeId)   const;

        void load(QStringList &errorList);
        void save() const;
        virtual void clear();

        virtual QJsonObject toJson() const;
        virtual void fromJson(const QJsonObject &src, QStringList &errorList);

    private:
        QStringList makeDepthIdList(const QString &id) const;
        entity::SharedScope getScopeWithDepthList(const QStringList &ids) const;
        void getDepthType(const entity::SharedScope &scope, const QString &id, entity::SharedType &result) const;
        QString makeFullPath() const;
        void recursiveFind(entity::SharedScope scope, const QString &id, QStringList &ids) const;

        QString        m_Name;
        QString        m_Path;
        entity::Scopes m_Scopes;
    };

} // namespace db

