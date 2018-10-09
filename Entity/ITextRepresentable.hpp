/*****************************************************************************
**
** Copyright (C) 2018 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 09-10-2018.
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

#include <Entity/EntityTypes.hpp>

namespace Entity {

    /// Convert objects from text and vice versa
    /**
     * A special DSL is used for each entity.
     * See documentation in implementers for further details.
     */
    class ITextRepresentable
    {
    public:
        virtual ~ITextRepresentable();

        /// Converts element to the text representation
        /**
         * @return string representation or QString::null if conversion is not possible
         */
        virtual QString toString() const noexcept = 0;

        /// Converts element from the text representation
        /**
         * @note the object will be unchanged if conversion failed
         * @return true on success; false otherwise
         */
        virtual bool fromString(const QString &s) const noexcept = 0;
    };

} // namespace Entity
