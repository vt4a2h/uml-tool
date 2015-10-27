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

    const QString INDENT("    ");
    const QString DOUBLE_INDENT(INDENT + INDENT);
    const QString ENUM_TEMPLATE("enum %class%%name%%type% {%values%};");
    const QString EXT_TYPE_TEMPLATE("%const%%name%%pl%%template_params%");
    const QString FIELD_TEMPLATE("%keywords%%type%%name%");
    const QString METHOD_TEMPLATE("%lhs_k%%r_type%%name%(%parameters%)%rhs_k%%const%");
    const QString UNION_TEMPLATE("union %name% {%variables%};");
    const QString CLASS_TEMPLATE("%kind%%name%%parents%{%section%};");
    const QString TEMPLATE_TEMPLATE("template <%template_parameters%>");
    const QString TEMPLATE("%template%");
    const QString SCOPE_TEMPLATE_HEADER("namespace %name%\n{\n\n%code%\n\n}");
    const QString SCOPE_TEMPLATE_SOURCE("namespace %name%\n{\n\n%code%\n}");

}
