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
#include "Constants.h"

#include <QJsonObject>

#include <Entity/Class.h>
#include <Entity/ClassMethod.h>
#include <Entity/field.h>
#include <Utility/helpfunctions.h>

#include "QtHelpers.h"

namespace Relationship {

    /**
     * @brief Association::Association
     */
    Association::Association()
        : Association(Common::ID::nullID(), Common::ID::nullID(), DB::WeakTypeSearchers())
    {
    }

    /**
     * @brief Association::Association
     * @param tailTypeId
     * @param headTypeId
     * @param globalDatabase
     * @param projectDatabase
     */
    Association::Association(const Common::ID &tailTypeId, const Common::ID &headTypeId,
                             const DB::WeakTypeSearchers &typeSearchers)
        : Relation(tailTypeId, headTypeId, typeSearchers)
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
        QString getterName(G_ASSERT(headClass())->name().toLower());

        auto getter = G_ASSERT(tailClass())->makeMethod(getterName);
        getter->setReturnTypeId(m_GetSetTypeId);
        getter->setConstStatus(true);
    }

    /**
     * @brief Association::makeSetter
     */
    void Association::makeSetter()
    {
        QString setterName(QString("set%1").arg(G_ASSERT(headClass())->name()));

        auto setter = G_ASSERT(tailClass())->makeMethod(setterName);
        auto param = setter->addParameter(QString("src_%1").arg(G_ASSERT(headClass())->name().toLower()), m_GetSetTypeId);
        param->setPrefix("");
    }

    /**
     * @brief Association::removeGetter
     */
    void Association::removeGetter()
    {
        G_ASSERT(tailClass())->removeMethods(G_ASSERT(headClass())->name().toLower());
    }

    /**
     * @brief Association::removeSetter
     */
    void Association::removeSetter()
    {
        G_ASSERT(tailClass())->removeMethods(QString("set%1").arg(G_ASSERT(headClass())->name()));
    }

    /**
     * @brief Association::FieldTypeId
     * @return
     */
    Common::ID Association::FieldTypeId() const
    {
        return m_FieldTypeId;
    }

    /**
     * @brief Association::setFieldTypeId
     * @param FieldTypeId
     */
    void Association::setFieldTypeId(const Common::ID &FieldTypeId)
    {
        m_FieldTypeId = FieldTypeId;
    }

    /**
     * @brief Association::GetSetTypeId
     * @return
     */
    Common::ID Association::GetSetTypeId() const
    {
        return m_GetSetTypeId;
    }

    /**
     * @brief Association::setGetSetTypeId
     * @param GetSetTypeId
     */
    void Association::setGetSetTypeId(const Common::ID &GetSetTypeId)
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

        Util::checkAndSet(src, "Get and set type ID", errorList, [&src, &errorList, this](){
            m_GetSetTypeId.fromJson(src["Get and set type ID"], errorList);
        });
        Util::checkAndSet(src, "Field type ID", errorList, [&src, &errorList, this](){
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
        G_ASSERT(tailClass())->addField(G_ASSERT(headClass())->name(), m_FieldTypeId);
    }

    /**
     * @brief Association::removeField
     */
    void Association::removeField()
    {
        G_ASSERT(tailClass())->removeField(G_ASSERT(headClass())->name());
    }

} // namespace relationship
