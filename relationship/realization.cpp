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

#include "realization.h"
#include "enums.h"
#include "Constants.h"

#include <QJsonObject>
#include <QJsonArray>

#include <entity/Class.h>
#include <entity/ClassMethod.h>
#include <utility/helpfunctions.h>

#include "qthelpers.h"

namespace relationship {

    /**
     * @brief Realization::Realization
     */
    Realization::Realization()
        : Realization(common::ID::nullID(), common::ID::nullID(), db::WeakTypeSearchers())
    {
    }

    /**
     * @brief Realization::Realization
     * @param tailTypeId
     * @param headTypeId
     * @param globalDatabase
     * @param projectDatabase
     */
    Realization::Realization(const common::ID &tailTypeId, const common::ID &headTypeId,
                             const db::WeakTypeSearchers &typeSearchers)
        : Generalization(tailTypeId, headTypeId, typeSearchers)
    {
        m_RelationType = RealizationRelation;
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const Realization &lhs, const Realization &rhs)
    {
        return static_cast<const Generalization&>(lhs).isEqual(rhs) &&
               Util::seqSharedPointerEq(lhs.m_Methods, rhs.m_Methods);
    }

    /**
     * @brief Realization::addMethods
     * @param methods
     */
    void Realization::addMethods(const entity::MethodsList &methods)
    {
        m_Methods = methods;
    }

    /**
     * @brief Realization::methods
     * @return
     */
    entity::MethodsList Realization::methods() const
    {
        return m_Methods;
    }

    /**
     * @brief Realization::toJson
     * @return
     */
    QJsonObject Realization::toJson() const
    {
        auto result = Generalization::toJson();

        QJsonArray methods;
        for (auto &&method : m_Methods)
            methods.append(method->toJson());
        result.insert("Methods", methods);

        return result;
    }

    /**
     * @brief Realization::fromJson
     * @param src
     * @param errorList
     */
    void Realization::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        Generalization::fromJson(src, errorList);

        m_Methods.clear();
        Util::checkAndSet(src, "Methods", errorList, [&src, &errorList, this](){
            if (src["Methods"].isArray()) {
                entity::SharedMethod method;
                for (auto &&value : src["Methods"].toArray()) {
                    method = std::make_shared<entity::ClassMethod>();
                    method->fromJson(value.toObject(), errorList);
                    m_Methods.append(method);
                }
            } else {
                errorList << "Error: \"Methods\" is not array";
            }
        });
    }

    /**
     * @brief Realization::isEqual
     * @param rhs
     * @return
     */
    bool Realization::isEqual(const Realization &rhs) const
    {
        return *this == rhs;
    }

    /**
     * @brief Realization::make
     */
    void Realization::make()
    {
        entity::SharedMethod m;
        entity::SharedClass head = std::dynamic_pointer_cast<entity::Class>(G_ASSERT(headClass()));
        Q_ASSERT_X(head, "Realization::make", "head class not found or not Class");
        for (auto &&method : m_Methods) {
            m = std::make_shared<entity::ClassMethod>(*method.get());
            m->setRhsIdentificator(entity::RhsIdentificator::PureVirtual);
            head->addExistsMethod(m);

            m = std::make_shared<entity::ClassMethod>(*method.get());
            m->setRhsIdentificator(entity::RhsIdentificator::Override);
            G_ASSERT(tailClass())->addExistsMethod(m);
        }
    }

    /**
     * @brief Realization::clear
     */
    void Realization::clear()
    {
        entity::SharedClass head = std::dynamic_pointer_cast<entity::Class>(G_ASSERT(headClass()));
        Q_ASSERT_X(head, "Realization::clear", "head class not found or not Class");
        for (auto method : m_Methods) {
            head->removeMethods(method->name());
            G_ASSERT(tailClass())->removeMethods(method->name());
        }
    }

} // namespace relationship
