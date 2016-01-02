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

namespace application {

//    /// The struct settings
//    struct Setting {
//        QString name;
//        QVariant defaultValue;
//    };

//    /**
//     * @brief value
//     * @param settings
//     * @param s
//     * @return
//     */
//    template<class ValueType>
//    ValueType value(const QSettings &settings, const Setting &s)
//    {
//        if (settings.contains(s.name))
//            return settings.value(s.name).value<ValueType>();
//        else
//            return s.defaultValue.value<ValueType>();
//    }

//    // Main window
//    static const QString groupMainWindow = "mainWindow";
//    static const Setting mwGeometry = { "size", QRect(0, 0, 640, 480) };

//    // Path
//    static const QString path = "path";
//    static const Setting pathGlobalDB = { "fullPath", QDir::currentPath() };
//    static const Setting nameGlobalDB = { "name"    , "global"            };

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
