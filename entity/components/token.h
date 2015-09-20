/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 18/09/2015.
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

#include <QStringList>

#include "components_types.h"

namespace components {

    /// The Token class
    class Token
    {
    public:
        Token();
        Token(const QString &token);
        Token(const Tokens &tokens);

        bool isEmpty() const;
        bool isSingle() const;
        bool isMulti() const;

        Tokens tokens() const;
        QString token() const;

        // Useful for testing
        std::string toStdString() const;

        // Useful for debuging
        void dump() const;

    private:
        enum  {IsEmpty, IsSingle, IsMulti} m_Tag;

        // Unfortunately we cannot use unnamed union with non-trivial types.
        union Data {
            Tokens m_Tokens;
            QString m_Token;

            Data() {}
            Data(const QString &token) : m_Token(token) {}
            Data(const Tokens &tokens) : m_Tokens(tokens) {}
            ~Data() {}
        };

        Data m_Data;
    };

} // namespace components
