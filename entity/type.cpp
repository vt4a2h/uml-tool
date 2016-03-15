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

#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QTextStream>

#include <utility/helpfunctions.h>
#include <helpers/generatorid.h>

#include "scope.h"
#include "enums.h"
#include "constants.h"

namespace entity {

    /**
     * @brief Type::Type
     */
    Type::Type()
        : Type(DEFAULT_NAME, common::ID::globalScopeID())
    {
    }

    /**
     * @brief Type::Type
     * @param name
     * @param scopeId
     * @param typeId
     */
    Type::Type(const QString &name, const common::ID &scopeId, const common::ID &typeId)
        : BasicElement(name, scopeId, typeId.isValid() ? typeId : GeneratorID::instance().genID() )
    {
        if (m_Name.isEmpty() || m_Name == DEFAULT_NAME)
            setBaseTypeName();
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
     * @brief Type::toJson
     * @return
     */
    QJsonObject Type::toJson() const
    {
        QJsonObject result = BasicElement::toJson();

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
        BasicElement::fromJson(src, errorList);
        // TODO: read Kind of type here!
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
        return rhs.hashType() == this->hashType()      &&
               m_Name        == rhs.m_Name             &&
                ( withTypeid ? m_Id == rhs.m_Id : true );
    }

    /**
     * @brief Type::generateUniqueName
     */
    void Type::setBaseTypeName()
    {
        m_Name = BASE_TYPE_NAME;
    }

} // namespace entity
