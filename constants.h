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

namespace {
    static constexpr char* GLOBAL_SCOPE_ID = (char*)"_global_scope";
    static constexpr char* PROJECT_GLOBAL_SCOPE_ID = (char*)"__project_global_scope";
    static constexpr char* LOCALE_TEMPLATE_SCOPE_ID = (char*)"__template_scope";
    static constexpr char* STUB_ID = (char*)"_stub_type";
    static constexpr char* VOID_ID = (char*)"_void";

    static constexpr char* DEFAULT_NAME = (char*)"__noname";
    static constexpr char* DEFAULT_DESCRIPTION = (char*)"_nothing";
    static constexpr char* BASE_TYPE_NAME= (char*)"type";

    static constexpr char* DEFAULT_DATABASE_NAME = (char*)"_database";
    static constexpr char* DEFAULT_DATABASE_PATH = (char*)"";
    static constexpr char* APPLICATION_DATABASEL_NAME = (char*)"qumldb";
    static constexpr char* APPLICATION_DATABASE_FULL_NAME = (char*)"qumldb.qutdb";
    static constexpr char* DEFAULT_DATABASE_EXTENSION = (char*)"qutdb";

    static constexpr char* QTYPE_HASH_ID      = (char*)"_q_hash";
    static constexpr char* QTYPE_MULTIHASH_ID = (char*)"_q_multihash";
    static constexpr char* QTYPE_LIST_ID      = (char*)"_q_list";
    static constexpr char* QTYPE_SET_ID       = (char*)"_q_set";
    static constexpr char* QTYPE_MULTISET_ID  = (char*)"_q_multiset";

    static constexpr char* CFG_FILE_NAME = (char*)"config.umtconfig";
    static constexpr char* DEFAULT_PROJECT_NAME = (char*)"empty_project";
    static constexpr char* DEFAULT_PROJECT_PATH = (char*)"empty_path";
    static constexpr char* PROJECT_FILE_EXTENTION = (char*)"qut";
    static constexpr char* DATABASE_FILE_EXTENTION = (char*)"qutdb";
}
