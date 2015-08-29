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

#include <QPair>

#include <models/componentsmodel.h>

#include "TestComponentSignatureParser.h"

namespace {

    const QVector<QPair<QString, bool>> fieldData =
    {
        {"int ",                                          false},
        {"int a",                                         true },
        {"const int",                                     false},
        {"foo::bar::int a",                               true },
        {"const foo::bar::int a",                         true },
        {"constfoo::bar::int a",                          true },
        {"scons foo::bar::int a",                         false},
        {"constt foo::bar::int a",                        false},
        {"static const foo::bar::int a",                  true },
        {"staticconst foo::bar::int a",                   false},
        {"static const std::foo::baz::int * const **& a", true },
        {"int *consta",                                   true },
        {"std::vector<int> vec",                          true },
        {"std::vector<int, MyAlloc> vec",                 true },
        {"std::vector<int,> vec",                         true },
        {"std::vector<int><int> vec",                     false},
        {"std::vector< vec",                              false},
        {"std::vector> vec",                              false},
        {"std::vector<class, const> vec",                 false},
        {"std::vector<std::vector> vec",                  true },
    };

}

TEST_F(SignatureParser, ParsingFieldSignature)
{
    for (auto &&testData: fieldData) {
        ASSERT_EQ(m_Parser->parse(testData.first, models::DisplayPart::Fields), testData.second)
            << "Signature: " << testData.first.toStdString().c_str();
    }
}
