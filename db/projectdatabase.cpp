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

#include "projectdatabase.h"

#include <QJsonObject>
#include <QJsonArray>

#include <entity/scope.h>

#include <relationship/relation.h>
#include <utility/helpfunctions.h>

#include "constants.h"

namespace {

    entity::SharedScope addGlobalScopeToProject(db::ProjectDatabase &projectDb,
                                                entity::Scopes &scopes)
    {
        QString globalScopeID = projectDb.defaultScopeID();

        scopes[globalScopeID] = std::make_shared<entity::Scope>("::");
        scopes[globalScopeID]->setId(globalScopeID);

        return scopes[globalScopeID];
    }

}

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
        : Database(src)
    {
        copyFrom(src);
    }

    /**
     * @brief ProjectDatabase::ProjectDatabase
     * @param name
     * @param path
     */
    ProjectDatabase::ProjectDatabase(const QString &name, const QString &path)
        : Database(name, path)
    {
        addGlobalScopeToProject(*this, m_Scopes);
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
    relationship::SharedRelation ProjectDatabase::getRelation(const QString &id) const
    {
        return (m_Relations.contains(id) ? m_Relations[id] : nullptr);
    }

    /**
     * @brief ProjectDatabase::addRelation
     * @param relation
     */
    void ProjectDatabase::addRelation(const relationship::SharedRelation &relation)
    {
        m_Relations.insert(relation->id(), relation);
    }

    /**
     * @brief ProjectDatabase::containsRelation
     * @param id
     * @return
     */
    bool ProjectDatabase::containsRelation(const QString &id) const
    {
        return m_Relations.contains(id);
    }

    /**
     * @brief ProjectDatabase::removeRelation
     * @param id
     */
    void ProjectDatabase::removeRelation(const QString &id)
    {
        m_Relations.remove(id);
    }

    /**
     * @brief ProjectDatabase::relations
     * @return
     */
    relationship::RelationsList ProjectDatabase::relations() const
    {
        return m_Relations.values();
    }

    /**
     * @brief ProjectDatabase::clear
     */
    void ProjectDatabase::clear()
    {
        Database::clear();
        m_Relations.clear();
    }

    /**
     * @brief ProjectDatabase::toJson
     * @return
     */
    QJsonObject ProjectDatabase::toJson() const
    {
        QJsonObject result(Database::toJson());

        QJsonArray relations;
        for (auto &&val : m_Relations.values())
            relations.append(val->toJson());
        result.insert("Relations", relations);

        QJsonArray positions;
        for (auto &&val : m_ItemsPos) {
            QJsonObject obj;
            obj["id"] = val.first;
            obj["x"]  = val.second.x();
            obj["y"]  = val.second.y();
            positions.append(obj);
        }
        result.insert("Positions", positions);

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

        utility::checkAndSet(src, "Relations", errorList, [&src, &errorList, this](){
            if (src["Relations"].isArray()) {
                relationship::SharedRelation relation;
                QJsonObject obj;
                for (auto &&val : src["Relations"].toArray()) {
                    obj = val.toObject();
                    utility::checkAndSet(obj, "Type", errorList,
                                         [&obj, &errorList, &relation, this](){
                        relation = utility::makeRelation(static_cast<relationship::RelationType>(obj["Type"].toInt()));
                        relation->setProjectDatabase(this);
                        relation->setGlobalDatabase(m_GlobalDatabase ? m_GlobalDatabase.get() : nullptr);
                        relation->fromJson(obj, errorList);
                        m_Relations.insert(relation->id(), relation);
                    });
                }
            } else {
                errorList << "Error: \"Relations\" is not array";
            }
        });

        utility::checkAndSet(src, "Positions", errorList, [&, this](){
            if (src["Positions"].isArray()) {
                QJsonObject obj;
                for (auto &&val : src["Positions"].toArray()) {
                    obj = val.toObject();
                    m_ItemsPos.append({obj["id"].toString(), {obj["x"].toDouble(), obj["y"].toDouble()}});
                }
            } else {
                errorList << "Error: \"Positions\" is not array";
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
     * @brief ProjectDatabase::setItemsPos
     * @param positions
     */
    void ProjectDatabase::setItemsPos(const ItemsPos &positions)
    {
        m_ItemsPos = positions;
    }

    /**
     * @brief ProjectDatabase::itemsPos
     * @return
     */
    ItemsPos ProjectDatabase::itemsPos() const
    {
        return m_ItemsPos;
    }

    /**
     * @brief ProjectDatabase::defaultScopeID
     * @return
     */
    QString ProjectDatabase::defaultScopeID() const
    {
        return m_ID + PROJECT_GLOBAL_SCOPE_ID;
    }

    /**
     * @brief ProjectDatabase::defaultScope
     * @return
     */
    entity::SharedScope ProjectDatabase::defaultScope() const
    {
        return m_Scopes[m_ID + PROJECT_GLOBAL_SCOPE_ID];
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
    }


} // namespace db
