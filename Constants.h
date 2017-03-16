/*****************************************************************************
**
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 03/11/2014.
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

namespace {
    static const QString GLOBAL = "global";
    static const QString LOCALE_TEMPLATE_SCOPE_ID = "__template_scope";
    static const QString STUB_ID = "_stub_type";
    static const QString VOID_ID = "_void";
    static const QString BOOL_ID = "bool";

    static const QString DEFAULT_NAME = "__noname";
    static const QString DEFAULT_DESCRIPTION = "_nothing";
    static const QString BASE_TYPE_NAME= "Type";

    static const QString DEFAULT_DATABASE_NAME = "_database";
    static const QString DEFAULT_DATABASE_PATH = "";
    static const QString APPLICATION_DATABASEL_NAME = "qumldb";
    static const QString APPLICATION_DATABASE_FULL_NAME = "qumldb.qutdb";
    static const QString DEFAULT_DATABASE_EXTENSION = "qutdb";

    static const QString CFG_FILE_NAME = "config.umtconfig";
    static const QString DEFAULT_PROJECT_NAME = "empty_project";
    static const QString DEFAULT_PROJECT_PATH = "empty_path";
    static const QString PROJECT_FILE_EXTENTION = "qut";
    static const QString DATABASE_FILE_EXTENTION = "qutdb";
}
