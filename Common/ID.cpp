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
#include "ID.h"

#include <QHash>

namespace Common
{

    /**
     * @brief ID::ID
     */
    ID::ID()
        : ID(nullID())
    {
    }

    /**
     * @brief ID::ID
     * @param value
     */
    ID::ID(quint64 value)
        : m_value(value)
    {
        static int type = qRegisterMetaType<Common::ID>("Common::ID");
        static bool comparators = QMetaType::registerComparators<Common::ID>();
        Q_UNUSED(type);
        Q_UNUSED(comparators);
    }

    /**
     * @brief operator >=
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator >=(const ID &lhs, const ID &rhs)
    {
        return (lhs > rhs) || (lhs == rhs);
    }

    /**
     * @brief operator <=
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator <=(const ID &lhs, const ID &rhs)
    {
        return (lhs < rhs) || (lhs == rhs);
    }

    /**
     * @brief Common::ID::operator !=
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator !=(const ID &lhs, const ID &rhs)
    {
       return !(lhs == rhs);
    }

    /**
     * @brief Common::ID::operator +
     * @param lhs
     * @param rhs
     * @return
     */
    ID operator+(const ID &lhs, const ID &rhs)
    {
        return ID(lhs.m_value + rhs.m_value);
    }

    /**
     * @brief ID::operator ++
     * @return
     */
    ID ID::operator ++(int)
    {
        return ID(m_value++);
    }

    /**
     * @brief operator +
     * @param lhs
     * @param val
     * @return
     */
    ID operator +(const ID &lhs, int val)
    {
        return ID(lhs.m_value + val);
    }

    /**
     * @brief operator <
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator <(const ID &lhs, const ID &rhs)
    {
        return lhs.m_value < rhs.m_value;
    }

    /**
     * @brief operator >
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator >(const ID &lhs, const ID &rhs)
    {
        return lhs.m_value > rhs.m_value;
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const ID &lhs, const ID &rhs)
    {
        return lhs.m_value == rhs.m_value;
    }

    /**
     * @brief ID::isValid
     * @return
     */
    bool ID::isValid() const
    {
        return *this != nullID();
    }

    /**
     * @brief ID::toString
     * @return
     */
    QString ID::toString() const
    {
        return QString::number(m_value);
    }

    /**
     * @brief ID::fromString
     * @param in
     * @return
     */
    bool ID::fromString(const QString &in)
    {
        bool result = false;
        m_value = in.toULongLong(&result);
        return result;
    }

    /**
     * @brief GeneratorID::defaultID
     * @return
     */
    ID ID::nullID()
    {
        return ID(0);
    }

    /**
     * @brief Returns first free IF for for project items. ID's from 0 to 4096 are reserved
     *        for Qt and C++ types. Can be used to init project id counter or for sanity check.
     * @return first free ID.
     */
    ID ID::firstFreeID()
    {
        return ID(4097);
    }

    /**
     * @brief From nullID to this value are placed some constants like globalScopeID.
     * @return
     */
    ID ID::firstNonConstID()
    {
        return ID(500);
    }

    /**
     * @brief GeneratorID::globalScopeID
     * @return
     */
    ID ID::globalScopeID()
    {
        return ID(nullID().value() + 1);
    }

    /**
     * @brief GeneratorID::stdScopeID
     * @return
     */
    ID ID::stdScopeID()
    {
        return ID(globalScopeID().value() + 1);
    }

    /**
     * @brief ID::globalDatabaseID
     * @return
     */
    ID ID::globalDatabaseID()
    {
        return ID(stdScopeID().value() + 1);
    }

    /**
     * @brief ID::localTemplateScopeID
     * @return
     */
    ID ID::localTemplateScopeID()
    {
        return ID(globalDatabaseID().value() + 1);
    }

    /**
     * @brief ID::projectScopeID
     * @return
     */
    ID ID::projectScopeID()
    {
        return ID(localTemplateScopeID().value() + 1);
    }

    /**
     * @brief ID::value
     * @return
     */
    quint64 ID::value() const
    {
        return m_value;
    }

    /**
     * @brief ID::setValue
     * @param value
     */
    void ID::setValue(const quint64 &value)
    {
        m_value = value;
    }

    /**
     * @brief ID::toJson
     * @return
     */
    QJsonValue ID::toJson() const
    {
        // Workaround for correct saving
        return QJsonValue(QString::number(m_value));
    }

    /**
     * @brief ID::fromJson
     * @param in
     * @param errors
     */
    void ID::fromJson(const QJsonValue &in, ErrorList &errors)
    {
        QString result = in.toString();
        if (result.isEmpty()) {
            errors << tr("Wrong entity ID");
            return;
        }

        bool ok = false;
        m_value = result.toULongLong(&ok);
        if (!ok)
            errors << tr("Cannot convert value to the appropriate type.");
    }

    void swap(ID & lsh, ID & rhs)
    {
        std::swap(lsh.m_value, rhs.m_value);
    }

    ID::ID(ID const &) = default;
    ID &ID::operator=(ID const &) = default;

    ID::ID(ID &&) noexcept = default;
    ID &ID::operator=(ID &&) noexcept = default;

    /**
     * @brief qHash
     * @param e
     * @return
     */
    uint qHash(const ID &e)
    {
        return ::qHash(e.value());
    }

} // namespace common
