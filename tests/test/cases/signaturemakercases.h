/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 13/08/2015.
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

#include "TestSignatureMaker.hpp"

namespace
{
    auto findType(const db::SharedDatabase &d, const QString &name)
    {
        entity::SharedType type;
        utility::find_if(d->scopes(), [&](auto &&scope){  type = scope->typeByName(name); return !!type; });

        return type;
    }
}

TEST_F(SignatureMaker, MakingFieldSignature)
{
    auto typeInt = findType(m_GlobalDatabase, "int");
    ASSERT_TRUE(!!typeInt);

    entity::SharedField field = std::make_shared<entity::Field>();
    field->setName("a");
    field->setTypeId(typeInt->id());

    QString actual = m_Maker->signature( field );
    QString expect = "int a";
//    ASSERT_EQ()
}

