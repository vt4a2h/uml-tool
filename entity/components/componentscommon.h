/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 29/08/2015.
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

/// Class components (methods, fields, properties)
namespace components {

    /// Field groups names actual for parsing
    enum class FieldGroupNames
    {
        LhsKeywords = 1,
        ConstStatus,
        Namespaces,
        Typename,
        TemplateArgs,
        PLC,
        Name,
        GroupsCount,
    };

    /// Methods groups names actual for parsing
    enum class MethodsGroupsNames
    {
        LhsKeywords = 1,
        ReturnType,
        Name,
        Arguments,
        Const,
        RhsKeywords,
        GroupsCount,
    };

    /// Property groups names actual for parsing
    enum class PropGroupNames
    {
        Type = 1,
        Name,
        Member,
        Getter,
        Setter,
        Resetter,
        Notifier,
        Revision,
        Designable,
        Scriptable,
        Stored,
        User,
        Constant,
        Final,
        GroupsCount,
    };

    /// Type groups names actual for parsing
    enum class TypeGroups
    {
        ConstStatus = 1,
        Namespaces,
        Typename,
        TemplateArgs,
        PLC,
        GroupsCount,
    };

    /// Method arguments
    enum class Argument
    {
        Type = 1,
        Name,
        GroupsCount,
    };

} // namespace components
