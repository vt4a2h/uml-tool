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

#include <entity/entity_types.hpp>

#include <common/ID.h>
#include <common/SharedFromThis.h>

#include "ITypeSearcher.h"
#include "IScopeSearcher.h"
#include "types.h"
#include "db_types.hpp"

/**
 *  @brief namespace db
 */
namespace db {

    /**
     * @brief The Database class
     */
    class Database
        : public QObject
        , public ITypeSearcher
        , public IScopeSearcher
        , public common::SharedFromThis<Database>
    {
        Q_OBJECT

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

        QString fullPath() const;

        virtual entity::SharedScope addScope(
            const QString &name = "", const common::ID &parentScopeId = common::ID::nullID());
        virtual entity::SharedScope addExistsScope(const entity::SharedScope &scope);
        entity::SharedScope chainScopeSearch(const QStringList& scopesNames) const;
        bool containsScope(const common::ID &id) const;
        bool anyScopes() const;
        void removeScope(const common::ID &id);

        void load(ErrorList &errorList);
        bool save() const;
        virtual void clear();

        virtual QJsonObject toJson() const;
        virtual void fromJson(const QJsonObject &src, QStringList &errorList);

        virtual bool isEqual(const Database &rhs) const;

        common::ID id() const;
        void setId(const common::ID &ID);

        bool valid() const;

    public: // ITypeSearcher overrides
        entity::SharedType typeByID(const common::ID &typeId) const override;
        entity::SharedType typeByName(const QString &name) const override;

    public: // IScopeSearcher overrides
        entity::SharedScope scope(const common::ID &id, bool searchInDepth = false) const override;
        entity::ScopesList  scopes() const override;

    public slots:
        void onScopeIDChanged(const common::ID &oldID, const common::ID &newID);

    protected:
        entity::SharedScope depthScopeSearch(const common::ID &id) const;
        virtual void copyFrom(const Database &src);
        virtual void moveFrom(Database &src);

        QString    m_Name ;
        QString    m_Path ;
        common::ID m_ID   ;
        bool       m_Valid;

        entity::Scopes m_Scopes;

    private:
        using IDList = QList<common::ID>;
        IDList makeDepthIdList(const common::ID &id) const;
        entity::SharedScope getScopeWithDepthList(const IDList &ids) const;
        QString makeFullPath() const;
        void recursiveFind(entity::SharedScope scope, const common::ID &id, IDList &ids) const;

        void connectScope(entity::Scope *scope, bool connect = true);
    };

} // namespace db

