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

#include <utility/helpfunctions.h>

namespace relationship {

    /**
     * @brief Node::Node
     */
    Node::Node()
        :Node(entity::EntityID::nullID())
    {
    }

    /**
     * @brief Node::Node
     * @param typeId
     * @param multiplicity
     */
    Node::Node(const entity::EntityID &typeId, Multiplicity multiplicity)
        : m_TypeId(typeId)
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
        return lhs.m_TypeId       == rhs.m_TypeId      &&
               lhs.m_Description  == rhs.m_Description &&
               lhs.m_Multiplicity == rhs.m_Multiplicity;
    }

    /**
     * @brief Node::typeId
     * @return
     */
    entity::EntityID Node::typeId() const
    {
        return m_TypeId;
    }

    /**
     * @brief Node::setTypeId
     * @param typeId
     */
    void Node::setTypeId(const entity::EntityID &typeId)
    {
        m_TypeId = typeId;
    }

    /**
     * @brief Node::toJson
     * @return
     */
    QJsonObject Node::toJson() const
    {
        QJsonObject result;

        result.insert("Type ID", m_TypeId.toJson());
        result.insert("Description", m_Description);
        result.insert("Multiplicity", m_Multiplicity);

        return result;
    }

    /**
     * @brief Node::fromJson
     * @param src
     * @param errorList
     */
    void Node::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        utility::checkAndSet(src, "Type ID", errorList, [&src, &errorList, this](){
            m_TypeId.fromJson(src["Type ID"], errorList);
        });
        utility::checkAndSet(src, "Description", errorList, [&src, this](){
            m_Description = src["Description"].toString();
        });
        utility::checkAndSet(src, "Multiplicity", errorList, [&src, this](){
            m_Multiplicity = static_cast<Multiplicity>(src["Multiplicity"].toInt());
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
