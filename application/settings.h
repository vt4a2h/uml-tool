/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 20/06/2015.
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

#include <QRect>
#include <QColor>
#include <QPair>
#include <QMetaType>

namespace application {

    /// Namespace settings (for now it's the easiest way to store/restore settings)
    namespace settings {

        // Main window settings
        QRect mainWindowGeometry();
        void writeMainWindowGeometry(const QRect &rect);

        // Database settings
        QString globalDbPath();
        void setGlobalDbPath(const QString &path);

        QString globalDbName();
        void setGlobalDbName(const QString &name);

        // Elements
        QColor elementColor(const QString &marker);
        void setElementColor(const QString &marker, const QColor &color);
    }

} // namespace application
