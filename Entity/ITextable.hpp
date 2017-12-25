/*****************************************************************************
**
** Copyright (C) 2017 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 25.
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
#pragma once

#include <QString>

namespace Entity {

    /// Used to convert to/from simple text representation used in properties sections
    class ITextable
    {
    public:
        virtual ~ITextable() {}

        /// \return text representation or QString::null if failed
        virtual QString toString() const = 0;

        /// \return true for success, false for fail
        virtual bool fromString(const QString &text) const = 0;
    };

} // namespace Entity
