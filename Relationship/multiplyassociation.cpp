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

#include "multiplyassociation.h"
#include "enums.h"
#include "Constants.h"

#include <QJsonObject>

#include <DB/Database.h>
#include <Entity/Class.h>
#include <Entity/ClassMethod.h>
#include <Entity/field.h>
#include <Utility/helpfunctions.h>

#include "QtHelpers.h"

namespace Relationship {

    /**
     * @brief MultiplyAssociation::MultiplyAssociation
     */
    MultiplyAssociation::MultiplyAssociation()
        : MultiplyAssociation(Common::ID::nullID(), Common::ID::nullID(), DB::WeakTypeSearchers())
    {
    }

    /**
     * @brief MultiplyAssociation::MultiplyAssociation
     * @param tailTypeId
     * @param headTypeId
     * @param globalDatabase
     * @param projectDatabase
     */
    MultiplyAssociation::MultiplyAssociation(const Common::ID &tailTypeId,
                                             const Common::ID &headTypeId,
                                             const DB::WeakTypeSearchers &typeSearchers)
        : Association(tailTypeId, headTypeId, typeSearchers)
        , m_ContainerClass(nullptr)
    {
        m_RelationType = MultiRelation;
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const MultiplyAssociation &lhs, const MultiplyAssociation &rhs)
    {
        return static_cast<const Association&>(lhs).isEqual(rhs) &&
               lhs.m_ContainerTypeId == rhs.m_ContainerTypeId    &&
               lhs.m_KeyTypeId       == rhs.m_KeyTypeId          &&
               (lhs.m_ContainerClass == rhs.m_ContainerClass || *lhs.m_ContainerClass == *rhs.m_ContainerClass);
    }

    /**
     * @brief MultiplyAssociation::make
     */
    void MultiplyAssociation::make()
    {
        Association::make();
        makeDeleter();
        makeGroupGetter();
    }

    /**
     * @brief MultiplyAssociation::clear
     */
    void MultiplyAssociation::clear()
    {
        Association::clear();
        removeDeleter();
        removeGroupGetter();
    }

    /**
     * @brief MultiplyAssociation::containerTypeId
     * @return
     */
    Common::ID MultiplyAssociation::containerTypeId() const
    {
        return m_ContainerTypeId;
    }

    /**
     * @brief MultiplyAssociation::setContainerTypeId
     * @param containerTypeId
     */
    void MultiplyAssociation::setContainerTypeId(const Common::ID &containerTypeId)
    {
        m_ContainerClass = findType(containerTypeId);
        Q_ASSERT_X(m_ContainerClass,
                   "MultiplyAssociation::setContainerTypeId",
                   "container class not found");

        m_ContainerTypeId = containerTypeId;
    }

    /**
     * @brief MultiplyAssociation::makeGetter
     */
    void MultiplyAssociation::makeGetter()
    {
        QString getterName(QString("get%1").arg(G_ASSERT(headClass())->name()));

        auto getter = G_ASSERT(tailClass())->makeMethod(getterName);
        getter->setReturnTypeId(m_GetSetTypeId);
        getter->setConstStatus(true);

        auto parameter = getter->addParameter("key", m_KeyTypeId);
        parameter->setPrefix("");
    }

    /**
     * @brief MultiplyAssociation::makeSetter
     */
    void MultiplyAssociation::makeSetter()
    {
        QString setterName(QString("add%1").arg(G_ASSERT(headClass())->name()));

        auto setter = G_ASSERT(tailClass())->makeMethod(setterName);
        auto param = setter->addParameter(QString("src_%1").arg(G_ASSERT(headClass())->name().toLower()), m_GetSetTypeId);
        param->setPrefix("");
    }

    /**
     * @brief MultiplyAssociation::makeField
     */
    void MultiplyAssociation::makeField()
    {
        Q_ASSERT_X(m_ContainerClass,
                   "MultiplyAssociation::makeField",
                   "container class not found");
        G_ASSERT(tailClass())->addField(m_ContainerClass->name(), containerTypeId());
    }

    /**
     * @brief MultiplyAssociation::makeDeleter
     */
    void MultiplyAssociation::makeDeleter()
    {
        QString deleterName(QString("remove%1").arg(G_ASSERT(headClass())->name()));

        auto deleter = G_ASSERT(tailClass())->makeMethod(deleterName);
        auto parameter = deleter->addParameter("key", m_KeyTypeId);
        parameter->setPrefix("");
    }

    /**
     * @brief MultiplyAssociation::makeGroupGetter
     */
    void MultiplyAssociation::makeGroupGetter()
    {
        QString groupGetterName(QString("%1s").arg(G_ASSERT(headClass())->name().toLower()));

        auto groupGetter = G_ASSERT(tailClass())->makeMethod(groupGetterName);
        groupGetter->setReturnTypeId(m_ContainerTypeId);
        groupGetter->setConstStatus(true);
    }

    /**
     * @brief MultiplyAssociation::removeGetter
     */
    void MultiplyAssociation::removeGetter()
    {
        G_ASSERT(tailClass())->removeMethods(QString("get%1").arg(G_ASSERT(headClass())->name()));
    }

    /**
     * @brief MultiplyAssociation::removeSetter
     */
    void MultiplyAssociation::removeSetter()
    {
        G_ASSERT(tailClass())->removeMethods(QString("add%1").arg(G_ASSERT(headClass())->name()));
    }

    /**
     * @brief MultiplyAssociation::removeField
     */
    void MultiplyAssociation::removeField()
    {
        Q_ASSERT_X(m_ContainerClass,
                   "MultiplyAssociation::removeField",
                   "container class not found");
        G_ASSERT(tailClass())->removeField(m_ContainerClass->name());
    }

    /**
     * @brief MultiplyAssociation::removeDeleter
     */
    void MultiplyAssociation::removeDeleter()
    {
        G_ASSERT(tailClass())->removeMethods(QString("remove%1").arg(G_ASSERT(headClass())->name()));
    }

    /**
     * @brief MultiplyAssociation::removeGroupGetter
     */
    void MultiplyAssociation::removeGroupGetter()
    {
        G_ASSERT(tailClass())->removeMethods(QString("%1s").arg(G_ASSERT(headClass())->name().toLower()));
    }

    /**
     * @brief MultiplyAssociation::keyTypeId
     * @return
     */
    Common::ID MultiplyAssociation::keyTypeId() const
    {
        return m_KeyTypeId;
    }

    /**
     * @brief MultiplyAssociation::setKeyTypeId
     * @param indexTypeId
     */
    void MultiplyAssociation::setKeyTypeId(const Common::ID &indexTypeId)
    {
        m_KeyTypeId = indexTypeId;
    }

    /**
     * @brief MultiplyAssociation::toJson
     * @return
     */
    QJsonObject MultiplyAssociation::toJson() const
    {
        QJsonObject result = Association::toJson();

        result.insert("Conatiner ID", m_ContainerTypeId.toJson());
        result.insert("Key ID", m_KeyTypeId.toJson());

        return result;
    }

    /**
     * @brief MultiplyAssociation::fromJson
     * @param src
     * @param errorList
     */
    void MultiplyAssociation::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        Util::checkAndSet(src, "Conatiner ID", errorList, [&src, &errorList, this](){
            m_ContainerTypeId.fromJson(src["Conatiner ID"], errorList);
            m_ContainerClass = findType(m_ContainerTypeId);
            Q_ASSERT_X(m_ContainerClass,
                       "MultiplyAssociation::fromJson",
                       "container class is not found");
        });

        Util::checkAndSet(src, "Key ID", errorList, [&src, &errorList, this](){
            m_KeyTypeId.fromJson(src["Key ID"], errorList);
        });
    }

    /**
     * @brief MultiplyAssociation::isEqual
     * @param rhs
     * @return
     */
    bool MultiplyAssociation::isEqual(const MultiplyAssociation &rhs) const
    {
        return *this == rhs;
    }

} // namespace relationship
