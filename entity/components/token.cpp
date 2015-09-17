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
#include "token.h"

#include <QDebug>

namespace components {

    /**
     * @brief Tokens::isSingle
     * @return
     */
    Token::Token()
        : m_Tag(IsEmpty)
    {}

    /**
     * @brief Tokens::Tokens
     * @param token
     */
    Token::Token(const QString &token)
        : m_Tag(IsSingle)
        , m_Data(token)
    {}

    /**
     * @brief Token::Token
     * @param tokens
     */
    Token::Token(const QStringList &tokens)
        : m_Tag(IsMulti)
        , m_Data(tokens)
    {}

    /**
     * @brief Token::isEmpty
     * @return
     */
    bool Token::isEmpty() const
    {
        return m_Tag == IsEmpty;
    }

    /**
     * @brief Token::isSingle
     * @return
     */
    bool Token::isSingle() const
    {
        return m_Tag == IsSingle;
    }

    /**
     * @brief Token::IsMulti
     * @return
     */
    bool Token::isMulti() const
    {
        return m_Tag == IsMulti;
    }

    /**
     * @brief Token::tokens
     * @return
     */
    QStringList Token::tokens() const
    {
        Q_ASSERT(isMulti());
        return m_Data.m_Tokens;
    }

    /**
     * @brief Token::token
     * @return
     */
    QString Token::token() const
    {
        Q_ASSERT(isSingle());
        return m_Data.m_Token;
    }

    /**
     * @brief Tokens::toStdString
     * @return
     */
    std::string Token::toStdString() const
    {
        switch (m_Tag) {
            case IsEmpty:
                return "";

            case IsSingle:
                return m_Data.m_Token.toStdString();

            case IsMulti:
                return m_Data.m_Tokens.join(QChar::Space).toStdString();

            default:
                return "";
        }
    }

    /**
     * @brief Tokens::dump
     */
    void Token::dump() const
    {
        switch (m_Tag) {
            case IsEmpty:
                qDebug() << "No token(s) here.";
                break;

            case IsSingle:
                qDebug() <<  "Token: " + m_Data.m_Token;
                break;

            case IsMulti:
                qDebug() << "Tokens: " + m_Data.m_Tokens.join(QChar::Space);
                break;

            default: ;
        }
    }

} // namespace components

