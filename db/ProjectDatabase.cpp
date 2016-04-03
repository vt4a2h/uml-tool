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

#include "ProjectDatabase.h"

#include <QJsonObject>
#include <QJsonArray>

#include <entity/scope.h>
#include <entity/property.h>

#include <common/id.h>

#include <gui/graphics/entity.h>

#include <relationship/relation.h>

#include <utility/helpfunctions.h>

#include "constants.h"

namespace db {

    /**
     * @brief ProjectDatabase::ProjectDatabase
     * @param src
     */
    ProjectDatabase::ProjectDatabase(ProjectDatabase &&src)
    {
        moveFrom(src);
    }

    /**
     * @brief ProjectDatabase::ProjectDatabase
     * @param src
     */
    ProjectDatabase::ProjectDatabase(const ProjectDatabase &src)
        : QObject(nullptr)
        , Database(src)
    {
        copyFrom(src);
    }

    /**
     * @brief ProjectDatabase::ProjectDatabase
     * @param name
     * @param path
     */
    ProjectDatabase::ProjectDatabase(const QString &name, const QString &path)
        : QObject(nullptr)
        , Database(name, path)
    {
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const ProjectDatabase &lhs, const ProjectDatabase &rhs)
    {
        return static_cast<const Database &>(lhs).isEqual(rhs)               &&
               utility::seqSharedPointerEq(lhs.m_Relations, rhs.m_Relations) &&
               (lhs.m_GlobalDatabase == rhs.m_GlobalDatabase || *lhs.m_GlobalDatabase == *rhs.m_GlobalDatabase);
    }

    /**
     * @brief ProjectDatabase::operator =
     * @param rhs
     * @return
     */
    ProjectDatabase &ProjectDatabase::operator =(ProjectDatabase &&rhs)
    {
        if (this != &rhs)
            moveFrom(rhs);

        return *this;
    }

    /**
     * @brief ProjectDatabase::operator =
     * @param rhs
     * @return
     */
    ProjectDatabase &ProjectDatabase::operator =(ProjectDatabase rhs)
    {
        moveFrom(rhs);
        return *this;
    }

    /**
     * @brief ProjectDatabase::getRelation
     * @param id
     * @return
     */
    relationship::SharedRelation ProjectDatabase::getRelation(const common::ID &id) const
    {
        return (m_Relations.contains(id) ? m_Relations[id] : nullptr);
    }

    /**
     * @brief ProjectDatabase::addRelation
     * @param relation
     */
    void ProjectDatabase::addRelation(const relationship::SharedRelation &relation)
    {
        relation->setTypeSearchers({m_GlobalDatabase, safeShared()});
        m_Relations[relation->id()] = relation;
        emit relationAdded();
    }

    /**
     * @brief ProjectDatabase::containsRelation
     * @param id
     * @return
     */
    bool ProjectDatabase::containsRelation(const common::ID &id) const
    {
        return m_Relations.contains(id);
    }

    /**
     * @brief ProjectDatabase::removeRelation
     * @param id
     */
    void ProjectDatabase::removeRelation(const common::ID &id)
    {
        if (m_Relations.remove(id) != 0)
            emit relationRemoved();
    }

    /**
     * @brief ProjectDatabase::relations
     * @return
     */
    relationship::RelationsList ProjectDatabase::relations() const
    {
        return m_Relations.values().toVector();
    }

    /**
     * @brief ProjectDatabase::registerGraphicsEntity
     * @param e
     */
    void ProjectDatabase::registerGraphicsEntity(const graphics::EntityPtr &e)
    {
        if (e) {
            m_GraphicsEntities[e->id()] = e;
            emit graphicsEntityRegistred(e);
        }
    }

    /**
     * @brief ProjectDatabase::unregisterGraphicsEntity
     * @param e
     */
    void ProjectDatabase::unregisterGraphicsEntity(const graphics::EntityPtr &e)
    {
        if (e) {
            m_GraphicsEntities.remove(e->id());
            emit graphicsEntityUnregistred(e);
        }
    }

    /**
     * @brief ProjectDatabase::clear
     */
    void ProjectDatabase::clear()
    {
        Database::clear();
        m_Relations.clear();
    }

    namespace
    {
        const QString relationsMark = "Relations";
        const QString positionsMark = "Positions";

        template <class Container>
        inline QJsonArray mapToJson(const Container &c)
        {
            QJsonArray result;
            for (auto &&val : c)
                result.append(val->toJson());

            return result;
        }
    }

    /**
     * @brief ProjectDatabase::toJson
     * @return
     */
    QJsonObject ProjectDatabase::toJson() const
    {
        QJsonObject result(Database::toJson());

        result[relationsMark] = mapToJson(m_Relations);
        result[positionsMark] = mapToJson(m_GraphicsEntities);

        return result;
    }

    /**
     * @brief ProjectDatabase::fromJson
     * @param src
     * @param errorList
     */
    void ProjectDatabase::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        Database::fromJson(src, errorList);

        // TODO: restore graphics objects first

        utility::checkAndSet(src, relationsMark, errorList, [&src, &errorList, this](){
            if (src[relationsMark].isArray()) {
                relationship::SharedRelation relation;
                QJsonObject obj;
                for (auto &&val : src[relationsMark].toArray()) {
                    obj = val.toObject();
                    utility::checkAndSet(obj, relationship::Relation::typeMark(), errorList,
                                         [&obj, &errorList, &relation, this](){
                        relation =
                            utility::makeRelation(
                                static_cast<relationship::RelationType>(
                                    obj[relationship::Relation::typeMark()].toInt()));
                        relation->setTypeSearchers({m_GlobalDatabase, safeShared()});
                        relation->fromJson(obj, errorList);
                        m_Relations.insert(relation->id(), relation);
                    });
                }
            } else {
                errorList << "Error: \"Relations\" is not array";
            }
        });


    }

    /**
     * @brief ProjectDatabase::isEqual
     * @param rhs
     * @return
     */
    bool ProjectDatabase::isEqual(const ProjectDatabase &rhs) const
    {
        return *this == rhs;
    }

    /**
     * @brief ProjectDatabase::onTypeUserAdded
     * @param tu
     */
    void ProjectDatabase::onTypeUserAdded(const entity::SharedTypeUser &tu)
    {
        if (G_ASSERT(tu))
            tu->setTypeSearcher(globalDatabase());
    }

    /**
     * @brief ProjectDatabase::addScope
     * @param name
     * @param parentScopeId
     * @return
     */
    entity::SharedScope ProjectDatabase::addScope(const QString &name,
                                                  const common::ID &parentScopeId)
    {
        auto result = Database::addScope(name, parentScopeId);
        G_CONNECT(result.get(), &entity::Scope::typeSearcherRequired,
                  this, &ProjectDatabase::onTypeUserAdded);

        return result;
    }

    /**
     * @brief ProjectDatabase::addExistsScope
     * @param scope
     * @return
     */
    entity::SharedScope ProjectDatabase::addExistsScope(const entity::SharedScope &scope)
    {
        Database::addExistsScope(scope);
        G_CONNECT(scope.get(), &entity::Scope::typeSearcherRequired,
                  this, &ProjectDatabase::onTypeUserAdded);

        return scope;
    }

    /**
     * @brief ProjectDatabase::copyFrom
     * @param src
     */
    void ProjectDatabase::copyFrom(const ProjectDatabase &src)
    {
        m_GlobalDatabase = src.m_GlobalDatabase; // shallow copy. ok
        utility::deepCopySharedPointerHash(src.m_Relations, m_Relations, &relationship::Relation::id);
    }

    /**
     * @brief ProjectDatabase::moveFrom
     * @param src
     */
    void ProjectDatabase::moveFrom(ProjectDatabase &src)
    {
        Database::moveFrom(src);
        m_GlobalDatabase = std::move(src.m_GlobalDatabase);
        m_Relations = src.m_Relations;
    }

    /**
     * @brief ProjectDatabase::installTypeSearchers
     */
    void ProjectDatabase::installTypeSearchers()
    {
        for (auto &&s : m_Scopes)
            for (auto &&c : s->types())
                if (c->hashType() == entity::Class::staticHashType())
                    for (auto &&p : c->properties())
                        p->setTypeSearcher(globalDatabase());
    }

    /**
     * @brief ProjectDatabase::globalDatabase
     * @return
     */
    db::SharedDatabase ProjectDatabase::globalDatabase() const
    {
        return m_GlobalDatabase;
    }

    /**
     * @brief ProjectDatabase::setGlobalDatabase
     * @param globalDatabase
     */
    void ProjectDatabase::setGlobalDatabase(const db::SharedDatabase &globalDatabase)
    {
        m_GlobalDatabase = globalDatabase;

        if (m_GlobalDatabase)
            installTypeSearchers();
    }


} // namespace db
