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

#include "Database.h"

#include <QDir>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTextStream>
#include <QDebug>

#include <Utility/helpfunctions.h>
#include <Helpers/entityhelpres.h>

#include <Entity/Scope.h>
#include <Common/ID.h>

#include "Constants.h"

namespace DB {

    /**
     * @brief Database::Database
     * @param src
     */
    Database::Database(Database &&src)
    {
        moveFrom(src);
    }

    /**
     * @brief Database::Database
     * @param src
     */
    Database::Database(const Database &src)
        : QObject(nullptr)
    {
        copyFrom(src);
    }

    /**
     * @brief Database::Database
     * @param name
     * @param path
     */
    Database::Database(const QString &name, const QString &path)
        : m_Name(name.isEmpty() ? DEFAULT_DATABASE_NAME : name)
        , m_Path(path)
        , m_ID(Common::ID::nullID())
        , m_Valid(false)
    {
    }

    /**
     * @brief Database::~Database
     */
    Database::~Database()
    {
    }

    /**
     * @brief Database::operator =
     * @param rhs
     * @return
     */
    Database &Database::operator =(Database &&rhs)
    {
        if (this != &rhs)
            moveFrom(rhs);

        return *this;
    }

    /**
     * @brief Database::operator =
     * @param rhs
     * @return
     */
    Database &Database::operator =(Database rhs)
    {
        moveFrom(rhs);
        return *this;
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const Database &lhs, const Database &rhs)
    {
        return lhs.m_Name == rhs.m_Name &&
               lhs.m_Path == rhs.m_Path &&
               lhs.m_ID   == rhs.m_ID   &&
               Util::seqSharedPointerEq(lhs.m_Scopes, rhs.m_Scopes);
    }

    /**
     * @brief Database::path
     * @return
     */
    QString Database::path() const
    {
        return m_Path;
    }

    /**
     * @brief Database::setPath
     * @param path
     */
    void Database::setPath(const QString &path)
    {
        m_Path = path.simplified();
    }

    /**
     * @brief Database::name
     * @return
     */
    QString Database::name() const
    {
        return m_Name;
    }

    /**
     * @brief Database::setName
     * @param name
     */
    void Database::setName(const QString &name)
    {
        m_Name = name;
    }

    /**
     * @brief Database::fullPath
     * @return
     */
    QString Database::fullPath() const
    {
        return makeFullPath();
    }

    /**
     * @brief Database::scope
     * @param id
     * @param searchInDepth
     * @return
     */
    Entity::SharedScope Database::scope(const Common::ID &id, bool searchInDepth) const
    {
        if (!searchInDepth) {
            auto it = m_Scopes.find(id);
            return it != m_Scopes.end() ? *it : nullptr;
        } else {
            return depthScopeSearch(id);
        }
    }

    /**
     * @brief Database::addScope
     * @param name
     * @param parentScopeId
     * @return
     */
    Entity::SharedScope Database::addScope(const QString &name, const Common::ID &parentScopeId)
    {
        Entity::SharedScope scope(nullptr);

        if (!parentScopeId.isValid()) {
            scope = std::make_shared<Entity::Scope>(name, Common::ID::globalScopeID());
            m_Scopes[scope->id()] = scope;
        } else {
            auto searchResults = std::move(makeDepthIdList(parentScopeId));
            if (!searchResults.isEmpty()) {
                auto depthScope = getScopeWithDepthList(searchResults);
                if (depthScope)
                    scope = depthScope->addChildScope(name);
            }
        }

        if (scope)
            connectScope(scope.get());

        return scope;
    }

    /**
     * @brief Database::addExistsScope
     * @param scope
     */
    Entity::SharedScope Database::addExistsScope(const Entity::SharedScope &scope)
    {
        if (!scope)
            return nullptr;

        Q_ASSERT(!m_Scopes.contains(scope->id()));
        m_Scopes[scope->id()] = scope;
        connectScope(scope.get());
        return scope;
    }

    /**
     * @brief Database::chainScopeSearch
     * @param scopesNames
     * @return
     */
    Entity::SharedScope Database::chainScopeSearch(const QStringList &scopesNames) const
    {
        return Entity::chainScopeSearch(m_Scopes, scopesNames);
    }

    /**
     * @brief Database::containsScope
     * @param id
     * @return
     */
    bool Database::containsScope(const Common::ID &id) const
    {
        return m_Scopes.contains(id);
    }

    /**
     * @brief Database::anyScopes
     * @return
     */
    bool Database::anyScopes() const
    {
        return !m_Scopes.isEmpty();
    }

    /**
     * @brief Database::removeScope
     * @param id
     */
    void Database::removeScope(const Common::ID &id)
    {
        auto it = m_Scopes.find(id);
        if (it != m_Scopes.end()) {
            connectScope(it->get(), false /*connect*/);
            m_Scopes.remove((*it)->id());
        }
    }

    /**
     * @brief Database::scopes
     * @return
     */
    Entity::ScopesList Database::scopes() const
    {
        return m_Scopes.values().toVector();
    }

    /**
     * @brief Database::onScopeIDChanged
     * @param oldID
     * @param newID
     */
    void Database::onScopeIDChanged(const Common::ID &oldID, const Common::ID &newID)
    {
        auto it = m_Scopes.find(oldID);
        if (!m_Scopes.contains(newID) && it != m_Scopes.end()) {
            auto scope = *it;
            m_Scopes.remove(oldID);

            Q_ASSERT(scope->id() == newID);

            m_Scopes[newID] = scope;
        } else {
            qWarning() << "Wrong new scope ID: " << newID.value() << ", old was: " << oldID.value();
        }
    }

    /**
     * @brief Database::depthScopeSearch
     * @param id
     * @return
     */
    Entity::SharedScope Database::depthScopeSearch(const Common::ID &id) const
    {
        return getScopeWithDepthList(makeDepthIdList(id));
    }

    namespace
    {
        template <class IDType>
        void getDepthType(const Entity::SharedScope &scope, const IDType &id,
                          Entity::SharedType &result)
        {
            if (scope->containsType(id)) {
                result = scope->type(id);
                return;
            } else if (scope->hasChildScopes()){
                for (auto &&child_scope : scope->scopes()) {
                    if (result)
                        break;
                    getDepthType(child_scope, id, result);
                }
            }
        }

        template <class IDType>
        Entity::SharedType typeSearchImpl(const IDType &typeId, const Entity::Scopes &scopes)
        {
            Entity::SharedType result;

            for (auto &&scope : scopes.values()) {
                getDepthType(scope, typeId, result);
                if (result)
                    break;
            }

            return result;
        }
    }

    /**
     * @brief Database::depthTypeSearch
     * @param typeId
     * @return
     */
    Entity::SharedType Database::typeByID(const Common::ID &typeId) const
    {
        return typeSearchImpl(typeId, m_Scopes);
    }

    /**
     * @brief Database::typeByName
     * @param typeId
     * @return
     */
    Entity::SharedType Database::typeByName(const QString &name) const
    {
        return typeSearchImpl(name, m_Scopes);
    }

    /**
     * @brief Database::makeDepthIdList
     * @param id
     * @return
     */
    Database::IDList Database::makeDepthIdList(const Common::ID &id) const
    {
        IDList result;

        if (containsScope(id)) {
            result << id;
        } else {
            for (auto scope : m_Scopes.values()) {
                recursiveFind(scope, id, result);
                if (!result.empty()) break;
            }
        }

        return result;
    }

    /**
     * @brief Database::getScopeWithDepthList
     * @param ids
     * @return
     */
    Entity::SharedScope Database::getScopeWithDepthList(const IDList &ids) const
    {
        Entity::SharedScope result(nullptr);
        if (!ids.isEmpty() && containsScope(ids[0])) {
            result = scope(ids[0]);
            for (int i = 1, size = ids.size(); i < size; ++i)
                if (result->containsChildScope(ids[i]))
                    result = result->getChildScope(ids[i]);
                else
                    break;
        }

        return result;
    }

    /**
     * @brief Database::load
     * @param errorList
     */
    void Database::load(ErrorList &errorList)
    {
        QFile f(makeFullPath());
        if (f.open(QIODevice::ReadOnly)) {
            QJsonParseError errorMessage;
            QJsonDocument jdoc(QJsonDocument::fromJson(f.readAll(), &errorMessage));

            if (errorMessage.error == QJsonParseError::NoError) {
                fromJson(jdoc.object(), errorList);
            } else {
                errorList << errorMessage.errorString();
            }
        } else {
            errorList << QObject::tr("Cannot load database: %1.").arg(f.fileName());
        }

        m_Valid = errorList.isEmpty();
    }

    /**
     * @brief Database::clear
     */
    void Database::clear()
    {
        m_Scopes.clear();
    }

    /**
     * @brief Database::save
     * @return
     */
    bool Database::save() const
    {
        if (!QDir(m_Path).exists())
            if (!QDir().mkpath(m_Path))
                return false;

        QFile f(makeFullPath());
        if (f.open(QIODevice::WriteOnly)) {
            QJsonDocument jdoc(toJson());
            QTextStream stream(&f);
            stream << jdoc.toJson();
            return true;
        }

        return false;
    }

    /**
     * @brief Database::toJson
     * @return
     */
    QJsonObject Database::toJson() const
    {
        QJsonArray scopes;
        for (auto &&scope : m_Scopes.values())
            scopes.append(scope->toJson());

        QJsonObject result;
        result.insert("Name", m_Name);
        result.insert("ID",   m_ID.toJson());
        result.insert("Scopes", scopes);

        return result;
    }

    /**
     * @brief Database::fromJson
     * @param src
     * @param errorList
     */
    void Database::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        clear();

        Util::checkAndSet(src, "Name", errorList, [&src, this](){
            m_Name = src["Name"].toString();
        });
        Util::checkAndSet(src, "ID", errorList, [&, this](){
            m_ID.fromJson(src["ID"], errorList);
        });

        Util::checkAndSet(src, "Scopes", errorList, [&src, &errorList, this](){
            if (src["Scopes"].isArray()) {
                for (auto &&val : src["Scopes"].toArray())
                    addScope()->fromJson(val.toObject(), errorList);
            } else {
                errorList << "Error: \"Scopes\" is not array";
            }
        });
    }

    /**
     * @brief Database::isEqual
     * @param rhs
     * @return
     */
    bool Database::isEqual(const Database &rhs) const
    {
        return *this == rhs;
    }

    /**
     * @brief Database::id
     * @return
     */
    Common::ID Database::id() const
    {
        return m_ID;
    }

    /**
     * @brief Database::setId
     * @param entotyID
     */
    void Database::setId(const Common::ID &ID)
    {
        m_ID = ID;
    }

    /**
     * @brief Database::valid
     * @return
     */
    bool Database::valid() const
    {
        return m_Valid;
    }

    /**
     * @brief Database::moveFrom
     * @param src
     */
    void Database::moveFrom(Database &src)
    {
        m_Name  = std::move(src.m_Name);
        m_Path  = std::move(src.m_Path);
        m_ID    = std::move(src.m_ID);
        m_Valid = std::move(src.m_Valid);

        m_Scopes = std::move(src.m_Scopes);
    }

    /**
     * @brief Database::copyFrom
     * @param src
     */
    void Database::copyFrom(const Database &src)
    {
        m_Name  = src.m_Name;
        m_Path  = src.m_Path;
        m_ID    = src.m_ID;
        m_Valid = src.m_Valid;

        Util::deepCopySharedPointerHash(src.m_Scopes, m_Scopes, &Entity::Scope::id);
    }

    /**
     * @brief Database::makeFullPath
     * @return
     */
    QString Database::makeFullPath() const
    {
        return QDir::toNativeSeparators(
                   QString("%1%2%3.%4").arg(
                        m_Path,
                        m_Path.isEmpty() || m_Path.endsWith(QDir::separator()) ? "" : "/",
                        m_Name.toLower(),
                        DEFAULT_DATABASE_EXTENSION
                   )
               );
    }

    /**
     * @brief Database::recursiveFind
     * @param scope
     * @param id
     * @param ids
     */
    void Database::recursiveFind(Entity::SharedScope scope, const Common::ID &id, IDList &ids) const
    {
        if (scope->scopes().isEmpty())  {
            ids.clear();
            return;
        }

        ids << scope->id();

        if (scope->containsChildScope(id)) {
            ids << scope->getChildScope(id)->id();
            return;
        } else {
            for (auto sc : scope->scopes()) recursiveFind(sc, id, ids);
        }
    }

    /**
     * @brief Database::connectScope
     * @param scope
     * @param connect
     */
    void Database::connectScope(Entity::Scope *scope, bool connect)
    {
        if (connect)
            G_CONNECT(scope, &Common::BasicElement::idChanged, this, &Database::onScopeIDChanged);
        else
            G_DISCONNECT(scope, &Common::BasicElement::idChanged, this, &Database::onScopeIDChanged);
    }

} // namespace db
