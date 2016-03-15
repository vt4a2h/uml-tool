/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 10/03/2016.
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
#include "basicelement.h"

#include <entity/itypeuser.h>

#include <utility/helpfunctions.h>

#include <helpers/generatorid.h>

#include "enums.h"

namespace common {

    using namespace entity;

    namespace {
        const QString nameMark = "Name";
        const QString idMark = "ID";
        const QString scopeIdMark = "Scope ID";
    }

    BasicElement::BasicElement(const QString &name, const ID &scopeId, const ID &id)
        : m_Name(name)
        , m_Id(id)
        , m_ScopeId(scopeId)
    {
    }

    /**
     * @brief BasicEntity::BasicEntity
     * @param src
     */
    BasicElement::BasicElement(const BasicElement &src)
        : QObject()
        , m_Name(src.m_Name)
        , m_Id(src.m_Id)
        , m_ScopeId(src.m_ScopeId)
    {
    }

    /**
     * @brief BasicEntity::BasicEntity
     * @param src
     * @param errorList
     */
    BasicElement::BasicElement(const QJsonObject &src, QStringList &errorList)
    {
        fromJson(src, errorList);
    }

    /**
     * @brief BasicEntity::operator =
     * @param rhs
     * @return
     */
    BasicElement &BasicElement::operator =(const BasicElement &rhs)
    {
        if (this != &rhs) {
            setName(rhs.name());
            m_Id = rhs.m_Id;
            m_ScopeId = rhs.m_ScopeId;
        }

        return *this;
    }

    /**
     * @brief BasicEntity::BasicEntity
     */
    BasicElement::BasicElement(const QString &name, const ID &id)
        : BasicElement(name, ID::nullID(), id)
    {
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const BasicElement &lhs, const BasicElement &rhs)
    {
        return lhs.m_Name == rhs.m_Name && lhs.m_Id == rhs.m_Id && lhs.m_ScopeId == rhs.m_ScopeId;
    }

    /**
     * @brief Basiccommon::ID
     * @return
     */
    ID BasicElement::id() const
    {
        return m_Id;
    }

    /**
     * @brief BasicEntity::setId
     * @param id
     */
    void BasicElement::setId(const ID &id)
    {
        m_Id = id;
    }

    /**
     * @brief BasicEntity::scopeId
     * @return
     */
    ID BasicElement::scopeId() const
    {
        return m_ScopeId;
    }

    /**
     * @brief BasicEntity::setScopeId
     * @param id
     */
    void BasicElement::setScopeId(const ID &id)
    {
        m_ScopeId = id;
    }

    /**
     * @brief BasicEntity::name
     * @return
     */
    QString BasicElement::name() const
    {
        return m_Name;
    }

    /**
     * @brief BasicEntity::setName
     * @param name
     */
    void BasicElement::setName(const QString &name)
    {
        if (m_Name != name) {
            auto oldName = m_Name;
            m_Name = name;

            emit nameChanged(oldName, m_Name);
        }
    }

    /**
     * @brief BasicEntity::writeToFile
     * @param fileName
     */
    void BasicElement::writeToFile(const QString &fileName) const
    {
        utility::writeToFile(*this, fileName);
    }

    /**
     * @brief BasicEntity::readFromFile
     * @param fileName
     * @return
     */
    bool BasicElement::readFromFile(const QString &fileName)
    {
        return utility::readFromFile(*this, fileName);
    }

    /**
     * @brief BasicEntity::toJson
     * @return
     */
    QJsonObject BasicElement::toJson() const
    {
        QJsonObject result;

        result.insert(nameMark, m_Name);

        result.insert(idMark, m_Id.toJson());
        result.insert(scopeIdMark, m_ScopeId.toJson());

        return result;
    }

    /**
     * @brief BasicEntity::fromJson
     * @param src
     * @param errorList
     */
    void BasicElement::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        using namespace utility;

        checkAndSet(src, nameMark, errorList, [&](){ m_Name = src[nameMark].toString(); });
        checkAndSet(src, idMark, errorList, [&](){ m_Id.fromJson(src[idMark], errorList); });
        checkAndSet(src, scopeIdMark, errorList, [&](){ m_ScopeId.fromJson(src[scopeIdMark], errorList); });
    }

    /**
     * @brief BasicEntity::hashType
     * @return
     */
    size_t BasicElement::hashType() const noexcept
    {
        return BasicElement::staticHashType();
    }

    /**
     * @brief BasicEntity::staticHashType
     * @return
     */
    size_t BasicElement::staticHashType() noexcept
    {
        return typeid(BasicElement).hash_code();
    }

    /**
     * @brief BasicEntity::marker
     * @return
     */
    QString BasicElement::marker() const noexcept
    {
        return BasicElement::staticMarker();
    }

    /**
     * @brief BasicEntity::staticMarker
     * @return
     */
    QString BasicElement::staticMarker() noexcept
    {
        return "BasicEntity";
    }

} // namespace common
