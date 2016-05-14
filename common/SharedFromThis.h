/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 14/05/2016.
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

#include <memory>
#include <qdebug.h>

namespace common {

    template <class T>
    struct SharedFromThis : public std::enable_shared_from_this<T>
    {
        std::shared_ptr<T> safeShared() noexcept
        {
            try {
                return this->shared_from_this();
            } catch (const std::exception &e) {
                qDebug() << "Cannot create shared pointer: " << e.what();
            } catch (...) {
                qDebug() << "Cannot create shared pointer: unhandled exception";
            }

            return nullptr;
        }
    };

} // common

