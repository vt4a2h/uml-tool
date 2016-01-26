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

#include "type.h"
#include "scope.h"
#include "enums.h"
#include "constants.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QTextStream>

#include <utility/helpfunctions.h>

namespace entity {

    /**
     * @brief Type::Type
     */
    Type::Type()
        : Type(DEFAULT_NAME, GLOBAL_SCOPE_ID)
    {
    }

    /**
     * @brief Type::Type
     * @param src
     */
    Type::Type(Type &&src)
        : BasicEntity(std::move(src))
    {
       moveFrom(std::move(src));
    }

    /**
     * @brief Type::Type
     * @param src
     */
    Type::Type(const Type &src)
        : BasicEntity(src)
    {
        copyFrom(src);
    }

    /**
     * @brief Type::Type
     * @param name
     * @param scopeId
     * @param typeId
     */
    Type::Type(const QString &name, const QString &scopeId, const QString &typeId)
        : BasicEntity(name, typeId.isEmpty() ? utility::genId() : typeId)
        , m_ScopeId(scopeId)
    {
        if (m_Name.isEmpty() || m_Name == DEFAULT_NAME)
            baseTypeName();
    }

    /**
     * @brief Type::operator =
     * @param rhs
     * @return
     */
    Type &Type::operator =(Type &&rhs)
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
    bool operator ==(const Type &lhs, const Type &rhs)
    {
        return lhs.isEqual(rhs);
    }

    /**
     * @brief Type::operator =
     * @param rhs
     * @return
     */
    Type &Type::operator =(const Type &rhs)
    {
        if (this != &rhs) {
            BasicEntity::operator =(rhs);
            copyFrom(rhs);
        }

        return *this;
    }

    /**
     * @brief Type::id
     * @return
     */
    QString Type::id() const
    {
        return m_Id;
    }

    /**
     * @brief Type::setId
     * @param id
     */
    void Type::setId(const QString &id)
    {
        m_Id = id;
    }

    /**
     * @brief Type::scopeId
     * @return
     */
    QString Type::scopeId() const
    {
        return m_ScopeId;
    }

    /**
     * @brief Type::setScopeId
     * @param scopeId
     */
    void Type::setScopeId(const QString &scopeId)
    {
        m_ScopeId = scopeId;
    }

    /**
     * @brief Type::toJson
     * @return
     */
    QJsonObject Type::toJson() const
    {
        QJsonObject result = BasicEntity::toJson();

        result.insert("Scope ID", m_ScopeId);
        result.insert("Kind of type", marker());

        return result;
    }

    /**
     * @brief Type::fromJson
     * @param src
     * @param errorList
     */
    void Type::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        BasicEntity::fromJson(src, errorList);
        utility::checkAndSet(src, "Scope ID", errorList, [&src, this](){
            m_ScopeId = src["Scope ID"].toString();
        });
    }

    /**
     * @brief Type::hashType
     * @return
     */
    size_t Type::hashType() const
    {
        return Type::staticHashType();
    }

    /**
     * @brief Type::staticHashType
     * @return
     */
    size_t Type::staticHashType()
    {
        return typeid(Type).hash_code();
    }

    /**
     * @brief Type::marker
     * @return
     */
    QString Type::marker() const
    {
        return Type::staticMarker();
    }

    /**
     * @brief Type::staticMarker
     * @return
     */
    QString Type::staticMarker()
    {
        return "Type";
    }

    /**
     * @brief Type::defaultName
     * @return
     */
    QString Type::defaultName() const
    {
        return staticDefaultName();
    }

    /**
     * @brief Type::staticDefaultName
     * @return
     */
    QString Type::staticDefaultName()
    {
       return tr("Type");
    }

    /**
     * @brief Type::isEqual
     * @param rhs
     * @return
     */
    bool Type::isEqual(const Type &rhs, bool withTypeid) const
    {
        return rhs.hashType() == this->hashType()       &&
                m_Name        == rhs.m_Name             &&
               ( withTypeid ? m_Id == rhs.m_Id : true ) &&
                m_ScopeId    == rhs.m_ScopeId;
    }

    /**
     * @brief Type::moveFrom
     * @param src
     */
    void Type::moveFrom(Type &&src)
    {
        m_ScopeId = std::move(src.m_ScopeId);
    }

    /**
     * @brief Type::copyFrom
     * @param src
     */
    void Type::copyFrom(const Type &src)
    {
        m_ScopeId = src.m_ScopeId;
    }

    /**
     * @brief Type::generateUniqueName
     */
    void Type::baseTypeName()
    {
        m_Name = BASE_TYPE_NAME;
    }

} // namespace entity
