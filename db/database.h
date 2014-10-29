#pragma once

#include "types.h"

/**
 *  @brief namespace db
 */
namespace db {

    /**
     * @brief The Database class
     */
    class Database
    {
    public:
        Database(Database &&src);
        Database(const Database &src);
        Database(const QString &name = "", const QString &path = "");
        virtual ~Database();

        Database &operator =(Database &&rhs);
        Database &operator =(Database rhs);

        friend bool operator ==(const Database &lhs, const Database &rhs);

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

        virtual bool isEqual(const Database &rhs) const;

    protected:
        virtual void copyFrom(const Database &src);
        virtual void moveFrom(Database &src);

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

