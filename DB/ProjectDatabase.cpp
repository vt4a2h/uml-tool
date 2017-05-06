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

#include <boost/range/algorithm/transform.hpp>

#include <QJsonObject>
#include <QJsonArray>

#include <Entity/Scope.h>
#include <Entity/Property.h>
#include <Entity/Class.h>

#include <Common/ID.h>

#include <GUI/graphics/Entity.h>
#include <GUI/graphics/GraphicsRelation.h>

#include <Relationship/Relation.h>
#include <Relationship/RelationFactory.h>

#include <Utility/helpfunctions.h>

#include "Constants.h"

namespace DB {

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
    }

    /**
     * @brief ProjectDatabase::~ProjectDatabase
     */
    ProjectDatabase::~ProjectDatabase()
    {
        if (m_ClearGraphics)
            qDeleteAll(graphicsEntities());
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
               Util::seqSharedPointerEq(lhs.m_Relations, rhs.m_Relations) &&
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
    // coverity[pass_by_value]
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
    Relationship::SharedRelation ProjectDatabase::getRelation(const Common::ID &id) const
    {
        return m_Relations.contains(id) ? m_Relations[id] : nullptr;
    }

    /**
     * @brief ProjectDatabase::addRelation
     * @param relation
     */
    void ProjectDatabase::addRelation(const Relationship::SharedRelation &relation)
    {
        relation->setTypeSearchers({m_GlobalDatabase, safeShared()});
        m_Relations[relation->id()] = relation;

        G_CONNECT(relation.get(), &Relationship::Relation::idChanged,
                  this, &ProjectDatabase::onRelationIDChanged);

        emit relationAdded();
    }

    /**
     * @brief ProjectDatabase::containsRelation
     * @param id
     * @return
     */
    bool ProjectDatabase::containsRelation(const Common::ID &id) const
    {
        return m_Relations.contains(id);
    }

    /**
     * @brief ProjectDatabase::removeRelation
     * @param id
     */
    void ProjectDatabase::removeRelation(const Common::ID &id)
    {
        if (auto relation = m_Relations[id]) {
            G_DISCONNECT(relation.get(), &Relationship::Relation::idChanged,
                         this, &ProjectDatabase::onRelationIDChanged);
            emit relationRemoved();
        }
    }

    /**
     * @brief ProjectDatabase::relations
     * @return
     */
    Relationship::RelationsList ProjectDatabase::relations() const
    {
        return m_Relations.values().toVector();
    }

    /**
     * @brief ProjectDatabase::graphicRelation
     * @param id
     * @return
     */
    Graphics::RelationPtr ProjectDatabase::graphicRelation(const Common::ID &id) const
    {
        return m_GraphicsRelations[id];
    }

    /**
     * @brief ProjectDatabase::registerGraphicsRelation
     * @param e
     */
    void ProjectDatabase::registerGraphicsRelation(const Graphics::RelationPtr &r)
    {
        if (r) {
            m_GraphicsRelations[r->id()] = r;
        }
    }

    /**
     * @brief ProjectDatabase::unregisterGraphicsRelation
     * @param e
     */
    void ProjectDatabase::unregisterGraphicsRelation(const Graphics::RelationPtr &r)
    {
        if (r) {
            m_GraphicsRelations.remove(r->id());
        }
    }

    /**
     * @brief ProjectDatabase::graphicRelations
     * @return
     */
    Graphics::RelationVector ProjectDatabase::graphicsRelations() const
    {
        return m_GraphicsRelations.values().toVector();
    }

    /**
     * @brief ProjectDatabase::graphicsEntity
     * @param id
     * @return
     */
    Graphics::EntityPtr ProjectDatabase::graphicsEntity(const Common::ID &id) const
    {
        return m_GraphicsEntities[id];
    }

    /**
     * @brief ProjectDatabase::registerGraphicsEntity
     * @param e
     */
    void ProjectDatabase::registerGraphicsEntity(const Graphics::EntityPtr &e)
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
    void ProjectDatabase::unregisterGraphicsEntity(const Graphics::EntityPtr &e)
    {
        if (e) {
            m_GraphicsEntities.remove(e->id());
            emit graphicsEntityUnregistred(e);
        }
    }

    /**
     * @brief ProjectDatabase::graphicEntities
     * @return
     */
    Graphics::EntityVector ProjectDatabase::graphicsEntities() const
    {
        return m_GraphicsEntities.values().toVector();
    }

    /**
     * @brief ProjectDatabase::graphicsItems
     * @return
     */
    Graphics::GraphicItems ProjectDatabase::graphicsItems() const
    {
        Graphics::GraphicItems result;
        result.reserve(m_GraphicsEntities.size() + m_GraphicsRelations.size());
        boost::range::transform(m_GraphicsEntities, std::back_inserter(result),
                                [](auto &&e) { return e.data(); });
        boost::range::transform(m_GraphicsRelations, std::back_inserter(result),
                                [](auto &&e) { return e.data(); });

        return result;
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

        Util::checkAndSet(src, relationsMark, errorList, [&src, &errorList, this](){
            if (src[relationsMark].isArray()) {
                auto &&factory = Relationship::RelationFactory::instance();
                for (auto &&val: src[relationsMark].toArray())
                    G_ASSERT(factory.make(val.toObject(), errorList));
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
    void ProjectDatabase::onTypeUserAdded(const Entity::SharedTypeUser &tu)
    {
        if (G_ASSERT(tu))
            tu->setTypeSearcher(globalDatabase());
    }

    /**
     * @brief ProjectDatabase::onRelationIDChanged
     * @param oldID
     * @param newID
     */
    void ProjectDatabase::onRelationIDChanged(const Common::ID &oldID, const Common::ID &newID)
    {
        if (auto relation = m_Relations[oldID]) {
            m_Relations.remove(oldID);
            m_Relations[newID] = relation;
        }
    }

    /**
     * @brief ProjectDatabase::addScope
     * @param name
     * @param parentScopeId
     * @return
     */
    Entity::SharedScope ProjectDatabase::addScope(const QString &name,
                                                  const Common::ID &parentScopeId)
    {
        auto result = Database::addScope(name, parentScopeId);
        G_CONNECT(result.get(), &Entity::Scope::typeSearcherRequired,
                  this, &ProjectDatabase::onTypeUserAdded);

        return result;
    }

    /**
     * @brief ProjectDatabase::addExistsScope
     * @param scope
     * @return
     */
    Entity::SharedScope ProjectDatabase::addExistsScope(const Entity::SharedScope &scope)
    {
        Database::addExistsScope(scope);
        G_CONNECT(scope.get(), &Entity::Scope::typeSearcherRequired,
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
        Util::deepCopySharedPointerHash(src.m_Relations, m_Relations, &Relationship::Relation::id);
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
                if (c->hashType() == Entity::Class::staticHashType())
                    for (auto &&p : c->properties())
                        p->setTypeSearcher(globalDatabase());
    }

    /**
     * @brief ProjectDatabase::getClearGraphics
     * @return
     */
    bool ProjectDatabase::getClearGraphics() const
    {
        return m_ClearGraphics;
    }

    /**
     * @brief ProjectDatabase::setClearGraphics
     * @param clear
     */
    void ProjectDatabase::setClearGraphics(bool clear)
    {
        m_ClearGraphics = clear;
    }

    /**
     * @brief ProjectDatabase::globalDatabase
     * @return
     */
    DB::SharedDatabase ProjectDatabase::globalDatabase() const
    {
        return m_GlobalDatabase;
    }

    /**
     * @brief ProjectDatabase::setGlobalDatabase
     * @param globalDatabase
     */
    void ProjectDatabase::setGlobalDatabase(const DB::SharedDatabase &globalDatabase)
    {
        m_GlobalDatabase = globalDatabase;

        if (m_GlobalDatabase)
            installTypeSearchers();
    }


} // namespace db
