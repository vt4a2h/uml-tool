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
#include "constants.h"

#include <QJsonObject>
#include <QJsonArray>

#include <entity/class.h>
#include <entity/classmethod.h>
#include <utility/helpfunctions.h>

namespace relationship {

    /**
     * @brief Realization::Realization
     */
    Realization::Realization()
        : Realization(STUB_ID, STUB_ID, nullptr, nullptr)
    {
    }

    /**
     * @brief Realization::Realization
     * @param tailTypeId
     * @param headTypeId
     * @param globalDatabase
     * @param projectDatabase
     */
    Realization::Realization(const QString &tailTypeId, const QString &headTypeId, db::Database *globalDatabase, db::Database *projectDatabase)
        : Generalization(tailTypeId, headTypeId, globalDatabase, projectDatabase)
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
               utility::seqSharedPointerEq(lhs.m_Methods, rhs.m_Methods);
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
        utility::checkAndSet(src, "Methods", errorList, [&src, &errorList, this](){
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
        entity::SharedClass head = std::dynamic_pointer_cast<entity::Class>(m_HeadClass);
        Q_ASSERT_X(head, "Realization::make", "head class not found or not Class");
        for (auto &&method : m_Methods) {
            m = std::make_shared<entity::ClassMethod>(*method.get());
            m->setRhsIdentificator(entity::PureVirtual);
            head->addExistsMethod(m);

            m = std::make_shared<entity::ClassMethod>(*method.get());
            m->setRhsIdentificator(entity::Override);
            m_TailClass->addExistsMethod(m);
        }
    }

    /**
     * @brief Realization::clear
     */
    void Realization::clear()
    {
        entity::SharedClass head = std::dynamic_pointer_cast<entity::Class>(m_HeadClass);
        Q_ASSERT_X(head, "Realization::clear", "head class not found or not Class");
        for (auto method : m_Methods) {
            head->removeMethods(method->name());
            m_TailClass->removeMethods(method->name());
        }
    }

} // namespace relationship
