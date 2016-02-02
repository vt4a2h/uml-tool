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
        return nullID() + 1;
    }

    /**
     * @brief GeneratorID::stdScopeID
     * @return
     */
    EntityID EntityID::stdScopeID()
    {
        return globalScopeID() + 1;
    }

    /**
     * @brief EntityID::globalDatabaseID
     * @return
     */
    EntityID EntityID::globalDatabaseID()
    {
        return stdScopeID() + 1;
    }

} // namespace entity
