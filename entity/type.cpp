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
#include "constants.cpp"

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
    {
       moveFrom(src);
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
        : BasicEntity(name)
        , m_Id(typeId.isEmpty() ? utility::genId() : typeId)
        , m_ScopeId(scopeId)
    {
        if (m_Name.isEmpty() || m_Name == DEFAULT_NAME)
            generateUniqueName();
    }

    /**
     * @brief Type::operator =
     * @param rhs
     * @return
     */
    Type &Type::operator =(Type &&rhs)
    {
       if (this != &rhs)
           moveFrom(rhs);

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
        return lhs.m_Name       == rhs.m_Name       &&
               lhs.m_Id         == rhs.m_Id         &&
               lhs.m_ScopeId    == rhs.m_ScopeId;
    }

    /**
     * @brief Type::operator =
     * @param rhs
     * @return
     */
    Type &Type::operator =(Type rhs)
    {
        moveFrom(rhs);

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
     * @brief Type::textMarker
     * @return
     */
    QString Type::textMarker() const
    {
        return "type";
    }

    /**
     * @brief Type::toJson
     * @return
     */
    QJsonObject Type::toJson() const
    {
        QJsonObject result;

        result.insert("Name", m_Name);
        result.insert("Scope ID", m_ScopeId);
        result.insert("ID", m_Id);
        result.insert("Kind of type", textMarker());

        return result;
    }

    /**
     * @brief Type::fromJson
     * @param src
     * @param errorList
     */
    void Type::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        utility::checkAndSet(src, "Name", errorList, [&src, this](){
            m_Name = src["Name"].toString();
        });
        utility::checkAndSet(src, "Scope ID", errorList, [&src, this](){
            m_ScopeId = src["Scope ID"].toString();
        });
        utility::checkAndSet(src, "ID", errorList, [&src, this](){
            m_Id = src["ID"].toString();
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
     * @brief Type::clone
     * @return
     */
    Type *Type::clone() const
    {
        return new Type(*this);
    }

    /**
     * @brief Type::isEqual
     * @param rhs
     * @return
     */
    bool Type::isEqual(const Type &rhs) const
    {
        return *this == rhs;
    }

    /**
     * @brief Type::moveFrom
     * @param src
     */
    void Type::moveFrom(Type &src)
    {
        m_Name    = std::move(src.m_Name);
        m_Id      = std::move(src.m_Id);
        m_ScopeId = std::move(src.m_ScopeId);
    }

    /**
     * @brief Type::copyFrom
     * @param src
     */
    void Type::copyFrom(const Type &src)
    {
        m_Id = src.m_Id;
        m_ScopeId = src.m_ScopeId;
    }

    /**
     * @brief Type::generateUniqueName
     */
    void Type::generateUniqueName()
    {
        // TODO: investigate why always "type"
        m_Name = "type";
    }

} // namespace entity
