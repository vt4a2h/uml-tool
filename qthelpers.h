/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 12/11/2015.
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

#include <QObject>

// Guarded connect/disconnect, assert in debug mode if action failed
#define G_CONNECT(arg, ...) G_ASSERT(QObject::connect(arg, ##__VA_ARGS__))
#define G_DISCONNECT(arg, ...) G_ASSERT(QObject::disconnect(arg, ##__VA_ARGS__))

// Guarded condition, assert in debug mode if condition is false
#define G_ASSERT(c) qthelpers::detail::g_assert(c, Q_FUNC_INFO, #c, __FILE__, __LINE__)

// Conditional G_ASSERT
#define G_ASSERT_C(c, cond) qthelpers::detail::g_assert_c(c, !!cond, Q_FUNC_INFO, \
                                                          #c, __FILE__, __LINE__)

namespace qthelpers
{
    namespace detail
    {
        template <class Condition>
        inline decltype(auto) g_assert(Condition &&c, const char *where,
                                       const char *what, const char *file, int line)
        {
        #ifdef QT_DEBUG
            if (!c)
                qt_assert_x(where, what, file, line);
        #else
            Q_UNUSED(where); Q_UNUSED(what); Q_UNUSED(file); Q_UNUSED(line);
        #endif

            return std::forward<Condition>(c);
        }

        template <class Condition>
        inline decltype(auto) g_assert_c(Condition &&c, bool cond, const char *where,
                                         const char *what, const char *file, int line)
        {
#ifdef QT_DEBUG
            if (cond && !c)
                qt_assert_x(where, what, file, line);
#else
            Q_UNUSED(cond); Q_UNUSED(where); Q_UNUSED(what); Q_UNUSED(file); Q_UNUSED(line);
#endif

            return std::forward<Condition>(c);
        }

    } // namespace detail
} // namespace qthelpers
