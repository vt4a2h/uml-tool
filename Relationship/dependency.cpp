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

#include "dependency.h"
#include "enums.h"
#include "Constants.h"

#include <QJsonObject>

#include <Entity/Class.h>
#include <Entity/ClassMethod.h>
#include <Utility/helpfunctions.h>

#include "QtHelpers.h"

namespace Relationship {

    /**
     * @brief Dependency::Dependency
     */
    Dependency::Dependency()
        : Dependency(Common::ID::nullID(), Common::ID::nullID(), DB::WeakTypeSearchers())
    {
    }

    /**
     * @brief Dependency::Dependency
     * @param tailTypeId
     * @param headTypeId
     * @param globalDatabase
     * @param projectDatabase
     */
    Dependency::Dependency(const Common::ID &tailTypeId, const Common::ID &headTypeId,
                           const DB::WeakTypeSearchers &typeSearchers)
        : Relation(tailTypeId, headTypeId, typeSearchers)
    {
        m_RelationType = DependencyRelation;
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const Dependency &lhs, const Dependency &rhs)
    {
        return static_cast<const Relation&>(lhs).isEqual(rhs) &&
               (lhs.m_Method == rhs.m_Method || *lhs.m_Method == *rhs.m_Method);
    }

    /**
     * @brief Dependency::make
     */
    void Dependency::make()
    {
        G_ASSERT(tailClass())->addExistsMethod(m_Method);
    }

    /**
     * @brief Dependency::clear
     */
    void Dependency::clear()
    {
        G_ASSERT(tailClass())->removeMethod(m_Method);
    }

    /**
     * @brief Dependency::method
     * @return
     */
    Entity::SharedMethod Dependency::method() const
    {
        return m_Method;
    }

    /**
     * @brief Dependency::setMethod
     * @param method
     */
    void Dependency::setMethod(const Entity::SharedMethod &method)
    {
        m_Method = method;
    }

    /**
     * @brief Dependency::toJson
     * @return
     */
    QJsonObject Dependency::toJson() const
    {
        auto result = Relation::toJson();

        result.insert("Method", m_Method->toJson());

        return result;
    }

    /**
     * @brief Dependency::fromJson
     * @param src
     * @param errorList
     */
    void Dependency::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        Relation::fromJson(src, errorList);

        Util::checkAndSet(src, "Method", errorList, [this, &src, &errorList](){
            QJsonObject obj = src["Method"].toObject();
            Util::checkAndSet(obj, "Type", errorList, [this, &obj, &errorList]{
                m_Method = Util::makeMethod(static_cast<Entity::ClassMethodType>(obj["Type"].toInt()));
                m_Method->fromJson(obj, errorList);
            });
        });
    }

    /**
     * @brief Dependency::isEqual
     * @param rhs
     * @return
     */
    bool Dependency::isEqual(const Dependency &rhs) const
    {
        return *this == rhs;
    }

} // namespace relationship
