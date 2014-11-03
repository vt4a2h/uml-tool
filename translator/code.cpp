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

#include "code.h"

namespace translator {

    /**
     * @brief Code::Code
     */
    Code::Code()
        : Code("", "")
    {}

    /**
     * @brief Code::Code
     * @param h
     * @param cpp
     */
    Code::Code(const QString &h, const QString &cpp)
        : toHeader(h)
        , toSource(cpp)
    {}

    /**
     * @brief Code::Code
     * @param src
     */
    Code::Code(const Code &src)
        : Code(src.toHeader, src.toSource)
    {}

    /**
     * @brief Code::join
     * @param code
     * @param sep
     */
    void Code::join(const Code &code, const QString &sep)
    {
        if (!code.toHeader.isEmpty())
            toHeader.append(sep).append(code.toHeader);
        if (!code.toSource.isEmpty())
            toSource.append(sep).append(code.toSource);
    }

    /**
     * @brief Code::isEmpty
     * @return
     */
    bool Code::isEmpty() const
    {
        return toHeader.isEmpty() && toSource.isEmpty();
    }

} // namespace translator
