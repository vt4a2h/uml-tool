/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 07/02/2015.
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
#include "icomponents.h"

#include <entity/enum.h>

namespace entity {

    /**
     * @brief IComponents::~IComponents
     */
    IComponents::~IComponents()
    {
    }

    /**
     * @brief IComponents::addNewMethod
     */
    SharedMethod IComponents::addNewMethod()
    {
        return entity::SharedMethod();
    }

    /**
     * @brief IComponents::methods
     * @return
     */
    MethodsList IComponents::methods() const
    {
        return MethodsList();
    }

    /**
     * @brief IComponents::fields
     * @return
     */
    FieldsList IComponents::fields() const
    {
        return FieldsList();
    }

    /**
     * @brief IComponents::variables
     * @return
     */
    VariablesList IComponents::variables() const
    {
        return VariablesList();
    }

} // end of namespace entity
