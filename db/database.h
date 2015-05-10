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

#pragma once

#include <entity/entity_types.hpp>

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
        void addExistsScope(const entity::SharedScope &scope);
        bool containsScope(const QString &id) const;
        bool anyScopes() const;
        void removeScope(const QString &id);
        entity::ScopesList scopes() const;

        entity::SharedScope depthScopeSearch(const QString &scopeId) const;
        entity::SharedType  depthTypeSearch(const QString &typeId)   const;

        void load(QStringList &errorList);
        bool save() const;
        virtual void clear();

        virtual QJsonObject toJson() const;
        virtual void fromJson(const QJsonObject &src, QStringList &errorList);

        virtual bool isEqual(const Database &rhs) const;

        QString id() const;

    protected:
        virtual void copyFrom(const Database &src);
        virtual void moveFrom(Database &src);

        QString        m_Name;
        QString        m_Path;
        QString        m_ID;
        entity::Scopes m_Scopes;

    private:
        QStringList makeDepthIdList(const QString &id) const;
        entity::SharedScope getScopeWithDepthList(const QStringList &ids) const;
        void getDepthType(const entity::SharedScope &scope, const QString &id, entity::SharedType &result) const;
        QString makeFullPath() const;
        void recursiveFind(entity::SharedScope scope, const QString &id, QStringList &ids) const;
    };

} // namespace db

