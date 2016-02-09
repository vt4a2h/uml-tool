/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 31/01/2016.
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
#include "entityid.h"

#include <QHash>
#include <QMetaType>

namespace entity
{

    /**
     * @brief EntityID::EntityID
     */
    EntityID::EntityID()
        : EntityID(nullID())
    {
    }

    /**
     * @brief EntityID::EntityID
     * @param value
     */
    EntityID::EntityID(quint64 value)
        : m_value(value)
    {
        static int type = qRegisterMetaType<entity::EntityID>("entity::EntityID");
        static bool comparators = QMetaType::registerComparators<entity::EntityID>();
        Q_UNUSED(type);
        Q_UNUSED(comparators);
    }

    /**
     * @brief operator <
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator <(const EntityID &lhs, const EntityID &rhs)
    {
        return lhs.m_value < rhs.m_value;
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const EntityID &lhs, const EntityID &rhs)
    {
        return lhs.m_value == rhs.m_value;
    }

    /**
     * @brief EntityID::isValid
     * @return
     */
    bool EntityID::isValid() const
    {
        return m_value != nullID();
    }

    /**
     * @brief EntityID::toString
     * @return
     */
    QString EntityID::toString() const
    {
        return QString::number(m_value);
    }

    /**
     * @brief EntityID::fromString
     * @param in
     * @return
     */
    bool EntityID::fromString(const QString &in)
    {
        bool result = false;
        m_value = in.toULongLong(&result);
        return result;
    }

    /**
     * @brief GeneratorID::defaultID
     * @return
     */
    EntityID EntityID::nullID()
    {
        return 0;
    }

    /**
     * @brief Returns first free IF for for project items. ID's from 0 to 4096 are reserved
     *        for Qt and C++ types. Can be used to init project id counter or for sanity check.
     * @return first free ID.
     */
    EntityID EntityID::firstFreeID()
    {
        return 4097;
    }

    /**
     * @brief From nullID to this value are placed some constants like globalScopeID.
     * @return
     */
    EntityID EntityID::firstNonConstID()
    {
        return 500;
    }

    /**
     * @brief GeneratorID::globalScopeID
     * @return
     */
    EntityID EntityID::globalScopeID()
    {
        return nullID().value() + 1;
    }

    /**
     * @brief GeneratorID::stdScopeID
     * @return
     */
    EntityID EntityID::stdScopeID()
    {
        return globalScopeID().value() + 1;
    }

    /**
     * @brief EntityID::globalDatabaseID
     * @return
     */
    EntityID EntityID::globalDatabaseID()
    {
        return stdScopeID().value() + 1;
    }

    /**
     * @brief EntityID::localTemplateScopeID
     * @return
     */
    EntityID EntityID::localTemplateScopeID()
    {
        return globalDatabaseID().value() + 1;
    }

    /**
     * @brief EntityID::projectScopeID
     * @return
     */
    EntityID EntityID::projectScopeID()
    {
        return localTemplateScopeID().value() + 1;
    }

    /**
     * @brief EntityID::voidID
     * @return
     */
    EntityID EntityID::voidID()
    {
        return projectScopeID().value() + 1;
    }

    /**
     * @brief EntityID::value
     * @return
     */
    quint64 EntityID::value() const
    {
        return m_value;
    }

    /**
     * @brief EntityID::setValue
     * @param value
     */
    void EntityID::setValue(const quint64 &value)
    {
        m_value = value;
    }

    /**
     * @brief EntityID::toJson
     * @return
     */
    QJsonValue EntityID::toJson() const
    {
        // Workaround for correct saving
        return QJsonValue(QString::number(m_value));
    }

    /**
     * @brief EntityID::fromJson
     * @param in
     * @param errors
     */
    void EntityID::fromJson(const QJsonValue &in, ErrorList &errors)
    {
        QString result = in.toString();
        if (result.isEmpty()) {
            errors << "Wrong entity ID";
            return;
        }

        bool ok = false;
        m_value = result.toULongLong(&ok);
        if (!ok)
            errors << "Cannot convert value to the appropriate type.";
    }

    /**
     * @brief qHash
     * @param e
     * @return
     */
    uint qHash(const entity::EntityID &e)
    {
        return ::qHash(e.value());
    }

} // namespace entity

