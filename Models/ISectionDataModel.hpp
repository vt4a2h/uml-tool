/*****************************************************************************
**
** Copyright (C) 2018 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 07-01-2018.
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

namespace Models {

    /// Date model for properties section
    class ISectionalDataModel
    {
    public:
        virtual ~ISectionalDataModel() {}

        /// Get text representation of data source
        virtual QString toText() const = 0;

        /// Update data source based on \p text
        /** Returns false if failed (you can check \p lastErrors() for details) **/
        virtual bool fromText(const QString &text) = 0;

        /// Returns human readable errors description
        virtual QStringList lastErrors() const = 0;
    };

} // Models
