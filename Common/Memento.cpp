/*****************************************************************************
**
** Copyright (C) 20186 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 21/09/2018.
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
#include "Memento.hpp"

#include <Common/BasicElement.h>

namespace Common {

    /**
     * @brief Memento::Memento
     * @param elem
     */
    Memento::Memento(const BasicElement &elem)
        : m_JsonObj(elem.toJson())
    {
        Q_ASSERT(!m_JsonObj.empty());
    }

    /**
     * @brief Memento::json
     * @return
     */
    QJsonObject Memento::json() const
    {
        return m_JsonObj;
    }

} // namespace Common
