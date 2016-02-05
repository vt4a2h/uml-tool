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

#include "association.h"
#include "enums.h"
#include "types.h"
#include "constants.h"

#include <QJsonObject>

#include <entity/class.h>
#include <entity/classmethod.h>
#include <entity/field.h>
#include <utility/helpfunctions.h>

namespace relationship {

    /**
     * @brief Association::Association
     */
    Association::Association()
        : Association(entity::EntityID::nullID(), entity::EntityID::nullID(), nullptr, nullptr)
    {
    }

    /**
     * @brief Association::Association
     * @param tailTypeId
     * @param headTypeId
     * @param globalDatabase
     * @param projectDatabase
     */
    Association::Association(const entity::EntityID &tailTypeId, const entity::EntityID &headTypeId,
                             db::Database *globalDatabase, db::Database *projectDatabase)
        : Relation(tailTypeId, headTypeId, globalDatabase, projectDatabase)
        , m_GetSetTypeId(headTypeId)
    {
        m_RelationType = AssociationRelation;
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const Association &lhs, const Association &rhs)
    {
        return static_cast<const Relation&>(lhs).isEqual(rhs) &&
               lhs.m_GetSetTypeId == rhs.m_GetSetTypeId &&
               lhs.m_FieldTypeId  == rhs.m_FieldTypeId;
    }

    /**
     * @brief Association::make
     */
    void Association::make()
    {
        makeField();
        makeGetter();
        makeSetter();
    }

    /**
     * @brief Association::clear
     */
    void Association::clear()
    {
        removeField();
        removeGetter();
        removeSetter();
    }

    /**
     * @brief Association::makeGetter
     */
    void Association::makeGetter()
    {
        QString getterName(m_HeadClass->name().toLower());

        auto getter = m_TailClass->makeMethod(getterName);
        getter->setReturnTypeId(m_GetSetTypeId);
        getter->setConstStatus(true);
    }

    /**
     * @brief Association::makeSetter
     */
    void Association::makeSetter()
    {
        QString setterName(QString("set%1").arg(m_HeadClass->name()));

        auto setter = m_TailClass->makeMethod(setterName);
        auto param = setter->addParameter(QString("src_%1").arg(m_HeadClass->name().toLower()), m_GetSetTypeId);
        param->setPrefix("");
    }

    /**
     * @brief Association::removeGetter
     */
    void Association::removeGetter()
    {
        m_TailClass->removeMethods(m_HeadClass->name().toLower());
    }

    /**
     * @brief Association::removeSetter
     */
    void Association::removeSetter()
    {
        m_TailClass->removeMethods(QString("set%1").arg(m_HeadClass->name()));
    }

    /**
     * @brief Association::FieldTypeId
     * @return
     */
    entity::EntityID Association::FieldTypeId() const
    {
        return m_FieldTypeId;
    }

    /**
     * @brief Association::setFieldTypeId
     * @param FieldTypeId
     */
    void Association::setFieldTypeId(const entity::EntityID &FieldTypeId)
    {
        m_FieldTypeId = FieldTypeId;
    }

    /**
     * @brief Association::GetSetTypeId
     * @return
     */
    entity::EntityID Association::GetSetTypeId() const
    {
        return m_GetSetTypeId;
    }

    /**
     * @brief Association::setGetSetTypeId
     * @param GetSetTypeId
     */
    void Association::setGetSetTypeId(const entity::EntityID &GetSetTypeId)
    {
        m_GetSetTypeId = GetSetTypeId;
    }

    /**
     * @brief Association::toJson
     * @return
     */
    QJsonObject Association::toJson() const
    {
        auto result = Relation::toJson();

        result.insert("Get and set type ID", m_GetSetTypeId.toJson());
        result.insert("Field type ID", m_FieldTypeId.toJson());

        return result;
    }

    /**
     * @brief Association::fromJson
     * @param src
     * @param errorList
     */
    void Association::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        Relation::fromJson(src, errorList);

        utility::checkAndSet(src, "Get and set type ID", errorList, [&src, &errorList, this](){
            m_GetSetTypeId.fromJson(src["Get and set type ID"], errorList);
        });
        utility::checkAndSet(src, "Field type ID", errorList, [&src, &errorList, this](){
            m_FieldTypeId.fromJson(src["Field type ID"], errorList);
        });
    }

    /**
     * @brief Association::isEqual
     * @param rhs
     * @return
     */
    bool Association::isEqual(const Association &rhs) const
    {
        return *this == rhs;
    }

    /**
     * @brief Association::makeField
     */
    void Association::makeField()
    {
        m_TailClass->addField(m_HeadClass->name(), m_FieldTypeId);
    }

    /**
     * @brief Association::removeField
     */
    void Association::removeField()
    {
        m_TailClass->removeField(m_HeadClass->name());
    }

} // namespace relationship
