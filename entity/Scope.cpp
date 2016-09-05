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

#include "Scope.h"
#include "Type.h"
#include "Class.h"
#include "TemplateClass.h"
#include "TemplateClassMethod.h"
#include "Union.h"
#include "Enum.h"
#include "enums.h"
#include "ExtendedType.h"
#include "constants.h"
#include "EntityFactory.h"

#include <utility>

#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include <QDebug>

#include <utility/helpfunctions.h>

#include <helpers/entityhelpres.h>
#include <helpers/GeneratorID.h>

namespace entity {

    /**
     * @brief Scope::Scope
     * @param src
     */
    Scope::Scope(Scope &&src)
        : BasicElement(std::move(src))
    {
        moveFrom(std::move(src));
    }

    /**
     * @brief Scope::Scope
     * @param src
     */
    Scope::Scope(const Scope &src)
        : BasicElement(src)
    {
        copyFrom(src);
    }

    /**
     * @brief Scope::Scope
     * @param scopeName
     * @param scopeId
     */
    Scope::Scope(const QString &scopeName, const common::ID &parentScopeID)
        : BasicElement(!scopeName.isEmpty() ? scopeName : DEFAULT_NAME,
                       parentScopeID.isValid() ? parentScopeID : common::ID::globalScopeID(),
                       helpers::GeneratorID::instance().genID())
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
            BasicElement::operator =(rhs);
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
            BasicElement::operator =(std::move(rhs));
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
        return static_cast<common::BasicElement const&>(lhs) ==
               static_cast<common::BasicElement const&>(rhs) &&
               utility::seqSharedPointerEq(lhs.m_Scopes, rhs.m_Scopes) &&
               utility::seqSharedPointerEq(lhs.m_Types, rhs.m_Types)   &&
               utility::seqSharedPointerEq(lhs.m_TypesByName, rhs.m_TypesByName);
    }

    /**
     * @brief Scope::getType
     * @param typeId
     * @return
     */
    SharedType Scope::type(const common::ID &typeId) const
    {
        return m_Types.value(typeId);
    }

    /**
     * @brief Scope::getType
     * @param typeId
     * @return
     */
    SharedType Scope::type(const common::ID &typeId)
    {
        return m_Types.value(typeId);
    }

    /**
     * @brief Scope::typeByName
     * @param name
     * @return
     */
    SharedType Scope::type(const QString &name) const
    {
        return m_TypesByName.value(name);
    }

    /**
     * @brief Scope::typeByName
     * @param name
     * @return
     */
    SharedType Scope::type(const QString &name)
    {
        return const_cast<const Scope*>(this)->type(name);
    }

    /**
     * @brief Scope::addExistsType
     * @param type
     */
    SharedType Scope::addExistsType(const SharedType &type)
    {
        type->setScopeId(m_Id);
        uniquifyName(*type, m_TypesByName.keys());

        Q_ASSERT(!m_Types.contains(type->id()));
        Q_ASSERT(!m_TypesByName.contains(type->name()));

        connectType(type.get());

        m_TypesByName[type->name()] = type;
        return *m_Types.insert(type->id(), type);
    }

    /**
     * @brief Scope::containsType
     * @param typeId
     * @return
     */
    bool Scope::containsType(const common::ID &typeId) const
    {
        return m_Types.contains(typeId);
    }

    /**
     * @brief Scope::containsType
     * @param name
     * @return
     */
    bool Scope::containsType(const QString &name) const
    {
        return m_TypesByName.contains(name);
    }

    /**
     * @brief Scope::removeType
     * @param typeId
     */
    void Scope::removeType(const common::ID &typeId)
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
        return m_Types.values().toVector();
    }

    /**
     * @brief Scope::getChildScope
     * @param typeId
     * @return
     */
    SharedScope Scope::getChildScope(const common::ID &typeId)
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
            scope->setScopeId(m_Id);
            m_Scopes[scope->id()] = scope;
        }
    }

    /**
     * @brief Scope::containsChildScope
     * @param typeId
     * @return
     */
    bool Scope::containsChildScope(const common::ID &typeId) const
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
    void Scope::removeChildScope(const common::ID &typeId)
    {
        m_Scopes.remove(typeId);
    }

    /**
     * @brief Scope::scopes
     * @return
     */
    ScopesList Scope::scopes() const
    {
        return m_Scopes.values().toVector();
    }

    /**
     * @brief Scope::toJson
     * @return
     */
    QJsonObject Scope::toJson() const
    {
        QJsonObject result = BasicElement::toJson();

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
        BasicElement::fromJson(src, errorList);

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
                auto const & factory = EntityFactory::instance();
                for (auto &&val : src["Types"].toArray())
                    G_ASSERT(factory.make(val.toObject(), errorList, id()));
            } else {
                errorList << "Error: \"Types\" is not array";
            }
        });

        Q_ASSERT(m_Types.count() == m_TypesByName.count());
    }

    /**
     * @brief Scope::onEntityNameChanged
     * @param oldName
     * @param newName
     */
    void Scope::onTypeNameChanged(const QString &oldName, const QString &newName)
    {
        if (!m_TypesByName.contains(newName) && m_TypesByName.contains(oldName)) {
            auto type = m_TypesByName[oldName];
            m_TypesByName.remove(oldName);

            Q_ASSERT(type->name() == newName);

            m_TypesByName[newName] = type;
            m_Types[type->id()] = type;
        } else {
            qWarning() << "Wrong new type name: " << newName << ", old was: " << oldName;
        }
    }

    /**
     * @brief Scope::onTypeIdChanged
     * @param oldID
     * @param newID
     */
    void Scope::onTypeIdChanged(const common::ID &oldID, const common::ID &newID)
    {
        auto it = m_Types.find(oldID);
        if (!m_Types.contains(newID) && it != m_Types.end()) {
            auto type = *it;
            m_Types.remove(oldID);

            Q_ASSERT(type->id() == newID);

            m_TypesByName[type->name()] = type;
            m_Types[newID] = type;
        } else {
            qWarning() << "Wrong new type ID: " << newID.value() << ", old was: " << oldID.value();
        }
    }

    /**
     * @brief Scope::onTemplateMethodAdded
     * @param m
     */
    void Scope::onTemplateMethodAdded(const SharedTemplateClassMethod &m)
    {
        connectTemplate(m.get());
    }

    /**
     * @brief Scope::copyFrom
     * @param src
     */
    void Scope::copyFrom(const Scope &src)
    {
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
        m_Scopes      = std::move(src.m_Scopes);
        m_Types       = std::move(src.m_Types );
        m_TypesByName = std::move(src.m_TypesByName);
    }

    /**
     * @brief Scope::connectTemplate
     * @param t
     */
    void Scope::connectTemplate(Template *t)
    {
        G_CONNECT(t,
                  &Template::requestUsingAdditionalScopeSearcher,
                  &EntityFactory::instance(),
                  &EntityFactory::addAdditionaScopeSearcher);
        G_CONNECT(t,
                  &Template::forgetAboutAdditionalScopeSearcher,
                  &EntityFactory::instance(),
                  &EntityFactory::removeAdditionaScopeSearcher);
    }

    /**
     * @brief Scope::connectType
     * @param t
     */
    void Scope::connectType(Type *t)
    {
        if (!t)
            return;

        // Keep old connection form to make code more generic without extracting connection
        // to the separate function and using enable_if
        if (t->hashType() == Class::staticHashType() ||
            t->hashType() == TemplateClass::staticHashType()) {
            G_CONNECT(t, SIGNAL(typeUserAdded(SharedTypeUser)),
                      this, SIGNAL(typeSearcherRequired(SharedTypeUser)));
            G_CONNECT(t, SIGNAL(templateMethodAdded(SharedTemplateClassMethod)),
                      this, SLOT(onTemplateMethodAdded(SharedTemplateClassMethod)));
        }

        if (t->hashType() == TemplateClass::staticHashType()) {
            connectTemplate(dynamic_cast<Template*>(t));
        }

        G_CONNECT(t, &common::BasicElement::nameChanged, this, &entity::Scope::onTypeNameChanged);
        G_CONNECT(t, &common::BasicElement::idChanged, this, &entity::Scope::onTypeIdChanged);
    }

} // namespace entity
