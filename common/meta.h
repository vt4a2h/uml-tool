/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 13/03/2016.
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

// The easiest way to implement it (template approach is not perfect in this case)
#define add_meta(type)                                                            \
    size_t hashType() const noexcept override { return type::staticHashType(); }  \
    static size_t staticHashType() noexcept {  return typeid(type).hash_code(); } \
                                                                                  \
    QString marker() const noexcept override { return type::staticMarker(); }     \
    static QString staticMarker() noexcept { return #type; }
