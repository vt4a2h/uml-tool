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

#include <QHash>

#include <Entity/EntityTypes.hpp>

#include <Common/ID.h>
#include <Common/SharedFromThis.h>

#include "ITypeSearcher.h"
#include "IScopeSearcher.h"
#include "types.h"
#include "DBTypes.hpp"

/**
 *  @brief namespace db
 */
namespace DB {

    /**
     * @brief The Database class
     */
    class Database
        : public QObject
        , public ITypeSearcher
        , public IScopeSearcher
        , public Common::SharedFromThis<Database>
    {
        Q_OBJECT

    public:
        Database(Database &&src) noexcept;
        Database(const Database &src);
        Database(const QString &name = "", const QString &path = "");
        virtual ~Database();

        Database &operator =(Database &&rhs) noexcept;
        Database &operator =(Database rhs);

        friend bool operator ==(const Database &lhs, const Database &rhs);

        QString path() const;
        void setPath(const QString &path);

        QString name() const;
        void setName(const QString &name);

        QString fullPath() const;

        virtual Entity::SharedScope addScope(
            const QString &name = "", const Common::ID &parentScopeId = Common::ID::nullID());
        virtual Entity::SharedScope addExistsScope(const Entity::SharedScope &scope);
        Entity::SharedScope chainScopeSearch(const QStringList& scopesNames) const;
        bool containsScope(const Common::ID &id) const;
        bool anyScopes() const;
        void removeScope(const Common::ID &id);

        void load(ErrorList &errorList);
        bool save() const;
        virtual void clear();

        virtual QJsonObject toJson() const;
        virtual void fromJson(const QJsonObject &src, QStringList &errorList);

        virtual bool isEqual(const Database &rhs) const;

        Common::ID id() const;
        void setId(const Common::ID &ID);

        bool valid() const;

        static QString mkPath(const QString &path, const QString &name);

        struct PathName { QString path; QString name; };
        static PathName splitPath(const QString &fullPath);

    public: // ITypeSearcher overrides
        Entity::SharedType typeByID(const Common::ID &typeId) const override;
        Entity::SharedType typeByName(const QString &name) const override;

    public: // IScopeSearcher overrides
        Entity::SharedScope scope(const Common::ID &id, bool searchInDepth = false) const override;
        Entity::ScopesList  scopes() const override;

    public slots:
        void onScopeIDChanged(const Common::ID &oldID, const Common::ID &newID);

    signals:
        void loaded();
        void scopeAdded(const Entity::SharedScope &scope);
        void scopeRemoved(const Entity::SharedScope &scope);

    protected:
        Entity::SharedScope depthScopeSearch(const Common::ID &id) const;
        virtual void copyFrom(const Database &src);
        virtual void moveFrom(Database &&src) noexcept;

        QString    m_Name ;
        QString    m_Path ;
        Common::ID m_ID   ;
        bool       m_Valid;

        Entity::Scopes m_Scopes;

    private:
        using IDList = QList<Common::ID>;
        IDList makeDepthIdList(const Common::ID &id) const;
        Entity::SharedScope getScopeWithDepthList(const IDList &ids) const;
        QString makeFullPath() const;
        void recursiveFind(Entity::SharedScope scope, const Common::ID &id, IDList &ids) const;

        void connectScope(Entity::Scope *scope, bool connect = true);
    };

} // namespace db

