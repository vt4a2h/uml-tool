/*****************************************************************************
**
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 29/10/2014.
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

#include "scope.h"
#include "type.h"
#include "class.h"
#include "templateclass.h"
#include "union.h"
#include "enum.h"
#include "enums.h"
#include "extendedtype.h"
#include "constants.h"

#include <utility>

#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

#include <utility/helpfunctions.h>

#include <helpers/entityhelpres.h>
#include <helpers/generatorid.h>

namespace entity {

    /**
     * @brief Scope::Scope
     * @param src
     */
    Scope::Scope(Scope &&src)
        : BasicEntity(std::move(src))
    {
        moveFrom(std::move(src));
    }

    /**
     * @brief Scope::Scope
     * @param src
     */
    Scope::Scope(const Scope &src)
        : BasicEntity(src)
    {
        copyFrom(src);
    }

    /**
     * @brief Scope::Scope
     * @param scopeName
     * @param scopeId
     */
    Scope::Scope(const QString &scopeName, const EntityID &parentScopeID)
        : BasicEntity(m_Name = !scopeName.isEmpty() ? scopeName : DEFAULT_NAME)
        , m_ParentScopeId(parentScopeID == GeneratorID::nullID() ? parentScopeID : GeneratorID::globalScopeID())
    {
    }

    /**
     * @brief Scope::operator =
     * @param rhs
     * @return
     */
    Scope &Scope::operator =(const Scope &rhs)
    {
        if (this != &rhs) {
            BasicEntity::operator =(rhs);
            copyFrom(rhs);
        }

        return *this;
    }

    /**
     * @brief Scope::operator =
     * @param rhs
     * @return
     */
    Scope &Scope::operator =(Scope &&rhs)
    {
        if (this != &rhs) {
            BasicEntity::operator =(std::move(rhs));
            moveFrom(std::move(rhs));
        }

        return *this;
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const Scope &lhs, const Scope &rhs)
    {
        return static_cast<BasicEntity const&>(lhs) == static_cast<BasicEntity const&>(rhs) &&
               lhs.m_ParentScopeId == rhs.m_ParentScopeId              &&
               utility::seqSharedPointerEq(lhs.m_Scopes, rhs.m_Scopes) &&
               utility::seqSharedPointerEq(lhs.m_Types, rhs.m_Types)   &&
               utility::seqSharedPointerEq(lhs.m_TypesByName, rhs.m_TypesByName);
    }

    /**
     * @brief Scope::getType
     * @param typeId
     * @return
     */
    SharedType Scope::type(const QString &typeId) const
    {
        return m_Types.value(typeId);
    }

    /**
     * @brief Scope::getType
     * @param typeId
     * @return
     */
    SharedType Scope::type(const QString &typeId)
    {
        return m_Types.value(typeId);
    }

    /**
     * @brief Scope::typeByName
     * @param name
     * @return
     */
    SharedType Scope::typeByName(const QString &name) const
    {
        return m_TypesByName.value(name);
    }

    /**
     * @brief Scope::typeByName
     * @param name
     * @return
     */
    SharedType Scope::typeByName(const QString &name)
    {
        return const_cast<const Scope*>(this)->typeByName(name);
    }

    /**
     * @brief Scope::addClonedType
     * @param type
     */
    void Scope::addClonedType(const SharedType &type)
    {
        SharedType newType(std::make_shared<Type>(*type));
        newType->setScopeId(m_Id);
        newType->setId(utility::genId());
        m_Types[newType->id()] = newType;
        m_TypesByName[newType->name()] = newType;
    }

    /**
     * @brief Scope::addExistsType
     * @param type
     */
    SharedType Scope::addExistsType(const SharedType &type)
    {
        type->setScopeId(m_Id);
        m_TypesByName[type->name()] = type;
        return *m_Types.insert(type->id(), type);
    }

    /**
     * @brief Scope::containsType
     * @param typeId
     * @return
     */
    bool Scope::containsType(const QString &typeId) const
    {
        return m_Types.contains(typeId);
    }

    /**
     * @brief Scope::removeType
     * @param typeId
     */
    void Scope::removeType(const QString &typeId)
    {
        auto type = m_Types.value(typeId);
        if (type)
            m_TypesByName.remove(type->name());

        m_Types.remove(typeId);
    }

    /**
     * @brief Scope::types
     * @return
     */
    TypesList Scope::types() const
    {
        return m_Types.values();
    }

    /**
     * @brief Scope::getChildScope
     * @param typeId
     * @return
     */
    SharedScope Scope::getChildScope(const QString &typeId)
    {
        return m_Scopes.value(typeId);
    }

    /**
     * @brief Scope::addChildScope
     * @param name
     * @return
     */
    SharedScope Scope::addChildScope(const QString &name)
    {
        SharedScope scope = std::make_shared<Scope>(name, m_Id);
        m_Scopes.insert(scope->id(), scope);
        return scope;
    }

    /**
     * @brief Scope::chainScopeSearch
     * @param scopesNames
     * @return
     */
    SharedScope Scope::chainScopeSearch(const QStringList &scopesNames) const
    {
        return entity::chainScopeSearch(m_Scopes, scopesNames);
    }

    /**
     * @brief Scope::addExistsChildScope
     * @param scope
     */
    void Scope::addExistsChildScope(const SharedScope &scope)
    {
        if (scope) {
            Q_ASSERT(!m_Scopes.contains(scope->id()));
            m_Scopes[scope->id()] = scope;
        }
    }

    /**
     * @brief Scope::containsChildScope
     * @param typeId
     * @return
     */
    bool Scope::containsChildScope(const QString &typeId) const
    {
        return m_Scopes.contains(typeId);
    }

    /**
     * @brief Scope::hasChildScopes
     * @return
     */
    bool Scope::hasChildScopes() const
    {
        return !m_Scopes.isEmpty();
    }

    /**
     * @brief Scope::removeChildScope
     * @param typeId
     */
    void Scope::removeChildScope(const QString &typeId)
    {
        m_Scopes.remove(typeId);
    }

    /**
     * @brief Scope::scopes
     * @return
     */
    ScopesList Scope::scopes() const
    {
        return m_Scopes.values();
    }

    /**
     * @brief Scope::id
     * @return
     */
    QString Scope::id() const
    {
        return m_Id;
    }

    /**
     * @brief Scope::setId
     * @param id
     */
    void Scope::setId(const QString &id)
    {
        m_Id = id;
    }

    /**
     * @brief Scope::parentScopeId
     * @return
     */
    EntityID Scope::parentScopeId() const
    {
        return m_ParentScopeId;
    }

    /**
     * @brief Scope::setParentScopeId
     * @param parentScopeId
     */
    void Scope::setParentScopeId(const EntityID &parentScopeId)
    {
       m_ParentScopeId = parentScopeId;
    }

    /**
     * @brief Scope::toJson
     * @return
     */
    QJsonObject Scope::toJson() const
    {
        QJsonObject result = BasicEntity::toJson();

        result.insert("Parent ID", QString::number(m_ParentScopeId));

        QJsonArray scopes;
        for (auto &&scope : m_Scopes.values())
            scopes.append(scope->toJson());
        result.insert("Scopes", scopes);

        QJsonArray types;
        for (auto &&type : m_Types.values())
            types.append(type->toJson());
        result.insert("Types", types);

        return result;
    }

    /**
     * @brief Scope::fromJson
     * @param src
     * @param errorList
     */
    void Scope::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        BasicEntity::fromJson(src, errorList);

        utility::checkAndSet(src, "Parent ID", errorList, [&src, this](){
                                  m_ParentScopeId = src["Parent ID"].toString().toULongLong(); });

        m_Scopes.clear();
        utility::checkAndSet(src, "Scopes", errorList, [&src, &errorList, this](){
            if (src["Scopes"].isArray()) {
                SharedScope scope;
                for (auto &&val : src["Scopes"].toArray()) {
                    scope = std::make_shared<Scope>();
                    scope->fromJson(val.toObject(), errorList);
                    m_Scopes.insert(scope->id(), scope);
                }
            } else {
                errorList << "Error: \"Scopes\" is not array";
            }
        });

        m_Types.clear();
        m_TypesByName.clear();
        utility::checkAndSet(src, "Types", errorList, [&src, &errorList, this](){
            if (src["Types"].isArray()) {
                SharedType type;
                QJsonObject obj;
                for (auto &&val : src["Types"].toArray()) {
                    obj = val.toObject();
                    utility::checkAndSet(obj, "Kind of type", errorList, [&obj, &type, &errorList, this](){
                        type = utility::makeType(obj.value("Kind of type").toString());
                        type->fromJson(obj, errorList);
                        m_Types[type->id()] = type;
                        m_TypesByName[type->name()] = type;
                    });
                }
            } else {
                errorList << "Error: \"Types\" is not array";
            }
        });
    }

    /**
     * @brief Scope::copyFrom
     * @param src
     */
    void Scope::copyFrom(const Scope &src)
    {
        m_ParentScopeId = src.m_ParentScopeId;

        utility::deepCopySharedPointerHash(src.m_Scopes, m_Scopes, &Scope::id);
        utility::deepCopySharedPointerHash(src.m_Types,  m_Types, &Type::id);
        utility::deepCopySharedPointerHash(src.m_TypesByName,  m_TypesByName, &Type::name);
    }

    /**
     * @brief Scope::moveFrom
     * @param src
     */
    void Scope::moveFrom(Scope &&src)
    {
        m_ParentScopeId = std::move(src.m_ParentScopeId);
        m_Scopes      = std::move(src.m_Scopes);
        m_Types       = std::move(src.m_Types );
        m_TypesByName = std::move(src.m_TypesByName);
    }

} // namespace entity
