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
#include "constants.h"

#include <QJsonObject>

#include <db/database.h>
#include <entity/class.h>
#include <entity/classmethod.h>
#include <entity/field.h>
#include <utility/helpfunctions.h>

namespace relationship {

    /**
     * @brief MultiplyAssociation::MultiplyAssociation
     */
    MultiplyAssociation::MultiplyAssociation()
        : MultiplyAssociation(STUB_ID, STUB_ID, nullptr, nullptr)
    {
    }

    /**
     * @brief MultiplyAssociation::MultiplyAssociation
     * @param tailTypeId
     * @param headTypeId
     * @param globalDatabase
     * @param projectDatabase
     */
    MultiplyAssociation::MultiplyAssociation(const QString &tailTypeId, const QString &headTypeId, db::Database *globalDatabase, db::Database *projectDatabase)
        : Association(tailTypeId, headTypeId, globalDatabase, projectDatabase)
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
    QString MultiplyAssociation::containerTypeId() const
    {
        return m_ContainerTypeId;
    }

    /**
     * @brief MultiplyAssociation::setContainerTypeId
     * @param containerTypeId
     */
    void MultiplyAssociation::setContainerTypeId(const QString &containerTypeId)
    {
        m_ContainerClass = tryToFindType(containerTypeId);
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
        QString getterName(QString("get%1").arg(m_HeadClass->name()));

        auto getter = m_TailClass->makeMethod(getterName);
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
        QString setterName(QString("add%1").arg(m_HeadClass->name()));

        auto setter = m_TailClass->makeMethod(setterName);
        auto param = setter->addParameter(QString("src_%1").arg(m_HeadClass->name().toLower()), m_GetSetTypeId);
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
        m_TailClass->addField(m_ContainerClass->name(), containerTypeId());
    }

    /**
     * @brief MultiplyAssociation::makeDeleter
     */
    void MultiplyAssociation::makeDeleter()
    {
        QString deleterName(QString("remove%1").arg(m_HeadClass->name()));

        auto deleter = m_TailClass->makeMethod(deleterName);
        auto parameter = deleter->addParameter("key", m_KeyTypeId);
        parameter->setPrefix("");
    }

    /**
     * @brief MultiplyAssociation::makeGroupGetter
     */
    void MultiplyAssociation::makeGroupGetter()
    {
        QString groupGetterName(QString("%1s").arg(m_HeadClass->name().toLower()));

        auto groupGetter = m_TailClass->makeMethod(groupGetterName);
        groupGetter->setReturnTypeId(m_ContainerTypeId);
        groupGetter->setConstStatus(true);
    }

    /**
     * @brief MultiplyAssociation::removeGetter
     */
    void MultiplyAssociation::removeGetter()
    {
        m_TailClass->removeMethods(QString("get%1").arg(m_HeadClass->name()));
    }

    /**
     * @brief MultiplyAssociation::removeSetter
     */
    void MultiplyAssociation::removeSetter()
    {
        m_TailClass->removeMethods(QString("add%1").arg(m_HeadClass->name()));
    }

    /**
     * @brief MultiplyAssociation::removeField
     */
    void MultiplyAssociation::removeField()
    {
        Q_ASSERT_X(m_ContainerClass,
                   "MultiplyAssociation::removeField",
                   "container class not found");
        m_TailClass->removeField(m_ContainerClass->name());
    }

    /**
     * @brief MultiplyAssociation::removeDeleter
     */
    void MultiplyAssociation::removeDeleter()
    {
        m_TailClass->removeMethods(QString("remove%1").arg(m_HeadClass->name()));
    }

    /**
     * @brief MultiplyAssociation::removeGroupGetter
     */
    void MultiplyAssociation::removeGroupGetter()
    {
        m_TailClass->removeMethods(QString("%1s").arg(m_HeadClass->name().toLower()));
    }

    /**
     * @brief MultiplyAssociation::keyTypeId
     * @return
     */
    QString MultiplyAssociation::keyTypeId() const
    {
        return m_KeyTypeId;
    }

    /**
     * @brief MultiplyAssociation::setKeyTypeId
     * @param indexTypeId
     */
    void MultiplyAssociation::setKeyTypeId(const QString &indexTypeId)
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

        result.insert("Conatiner ID", m_ContainerTypeId);
        result.insert("Key ID", m_KeyTypeId);

        return result;
    }

    /**
     * @brief MultiplyAssociation::fromJson
     * @param src
     * @param errorList
     */
    void MultiplyAssociation::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        utility::checkAndSet(src, "Conatiner ID", errorList, [&src, this](){
            m_ContainerTypeId = src["Conatiner ID"].toString();
            m_ContainerClass = tryToFindType(m_ContainerTypeId);
            Q_ASSERT_X(m_ContainerClass,
                       "MultiplyAssociation::fromJson",
                       "container class is not found");
        });

        utility::checkAndSet(src, "Key ID", errorList, [&src, this](){
            m_KeyTypeId = src["Key ID"].toString();
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
