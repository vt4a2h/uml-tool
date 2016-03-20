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

#include "node.h"
#include "enums.h"
#include "constants.h"

#include <QJsonObject>

#include <entity/type.h>

#include <utility/helpfunctions.h>

namespace relationship {

    namespace {
        const QString multMark  = "Multiplicity";
        const QString descrMark = "Description";
    }

    /**
     * @brief Node::Node
     */
    Node::Node()
        :Node(nullptr)
    {
    }

    /**
     * @brief Node::Node
     * @param typeId
     * @param multiplicity
     */
    Node::Node(const entity::SharedType &type, Multiplicity multiplicity)
        : m_Type(type)
        , m_Description(DEFAULT_DESCRIPTION)
        , m_Multiplicity(multiplicity)
    {
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const Node &lhs, const Node &rhs)
    {
        return utility::sharedPtrEq(lhs.m_Type, rhs.m_Type) &&
               lhs.m_Description  == rhs.m_Description &&
               lhs.m_Multiplicity == rhs.m_Multiplicity;
    }

    /**
     * @brief Node::toJson
     * @return
     */
    QJsonObject Node::toJson() const
    {
        QJsonObject result;

        result.insert(typeIDMark(), m_Type ? m_Type->id().toJson() : common::ID().toJson());
        result.insert(descrMark, m_Description);
        result.insert(multMark, m_Multiplicity);

        return result;
    }

    /**
     * @brief Node::fromJson
     * @param src
     * @param errorList
     */
    void Node::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        utility::checkAndSet(src, descrMark, errorList, [&src, this](){
            m_Description = src[descrMark].toString();
        });
        utility::checkAndSet(src, multMark, errorList, [&src, this](){
            m_Multiplicity = static_cast<Multiplicity>(src[multMark].toInt());
        });
    }

    /**
     * @brief Node::writeToFile
     * @param fileName
     */
    void Node::writeToFile(const QString &fileName) const
    {
        utility::writeToFile(*this, fileName);
    }

    /**
     * @brief Node::readFromFile
     * @param fileName
     * @return
     */
    bool Node::readFromFile(const QString &fileName)
    {
       return utility::readFromFile(*this, fileName);
    }

    /**
     * @brief Node::type
     * @return
     */
    entity::SharedType Node::type() const
    {
        return m_Type;
    }

    /**
     * @brief Node::setType
     * @param type
     */
    void Node::setType(const entity::SharedType &type)
    {
        m_Type = type;
    }

    /**
     * @brief Node::typeIDMark
     * @return
     */
    QString Node::typeIDMark()
    {
        return "Type ID";
    }

    /**
     * @brief Node::multiplicity
     * @return
     */
    Multiplicity Node::multiplicity() const
    {
        return m_Multiplicity;
    }

    /**
     * @brief Node::setMultiplicity
     * @param multiplicity
     */
    void Node::setMultiplicity(const Multiplicity &multiplicity)
    {
        m_Multiplicity = multiplicity;
    }

    /**
     * @brief Node::description
     * @return
     */
    QString Node::description() const
    {
        return m_Description;
    }

    /**
     * @brief Node::setDescription
     * @param description
     */
    void Node::setDescription(const QString &description)
    {
        m_Description = description;
    }

} // namespace relationship
