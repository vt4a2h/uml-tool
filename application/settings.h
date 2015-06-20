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

#include <QVariant>
#include <QSettings>

#include "constants.h"

namespace application {

    struct Setting {
        QString name;
        QVariant defaultValue;
    };

    // Main window
    static const QString groupMainWindow = "mainWindow";
    static const Setting mwGeometry = { "size", QRect(0, 0, 640, 480) };

    // Path
    static const QString path = "path";
    static const Setting pathGlobalDB = { QString("db.%1").arg(DATABASE_FILE_EXTENTION), "" };

} // namespace application
