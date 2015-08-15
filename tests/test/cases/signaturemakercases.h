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

    // type and name
    entity::SharedField field = std::make_shared<entity::Field>();
    field->setName("a");
    field->setTypeId(typeInt->id());

    QString actual = m_Maker->signature(field);
    QString expect = "int a";
    ASSERT_EQ(actual, expect);

    // const type name
    auto constInt = m_Scope->addType<entity::ExtendedType>();
    constInt->setTypeId(typeInt->id());
    constInt->setConstStatus(true);
    field->setTypeId(constInt->id());

    actual = m_Maker->signature(field);
    expect = "const int a";
    ASSERT_EQ(actual, expect);

    // const ptr to const
    constInt->addPointerStatus(true /*pointer to const*/);
    actual = m_Maker->signature(field);
    expect = "const int * const a";
    ASSERT_EQ(actual, expect);

    // mutable
    field->setTypeId(typeInt->id());
    field->addKeyword(entity::FieldKeyword::Mutable);
    actual = m_Maker->signature(field);
    expect = "mutable int a";
    ASSERT_EQ(actual, expect);

    // std
    auto deque = findType(m_GlobalDatabase, "deque");
    ASSERT_TRUE(!!deque);
    field->setTypeId(deque->id());
    field->removeKeyword(entity::FieldKeyword::Mutable);
    actual = m_Maker->signature(field);
    expect = "std::deque a";
    ASSERT_EQ(actual, expect);

    // with template parameters
    auto unMap = findType(m_GlobalDatabase, "unordered_map");
    ASSERT_TRUE(!!unMap);
    auto str = findType(m_GlobalDatabase, "string");
    ASSERT_TRUE(!!str);
    auto extMap = m_Scope->addType<entity::ExtendedType>();
    extMap->setTypeId(unMap->id());
    extMap->addTemplateParameter(str->id());
    extMap->addTemplateParameter(typeInt->id());
    field->setTypeId(extMap->id());

    actual = m_Maker->signature(field);
    expect = "std::unordered_map<std::string, int> a";
    ASSERT_EQ(actual, expect);
}

TEST_F(SignatureMaker, MakingMethodSignature)
{
    auto typeInt = findType(m_GlobalDatabase, "int");
    ASSERT_TRUE(!!typeInt);

    // simple field
    auto method = std::make_shared<entity::ClassMethod>("getInt");
    method->setReturnTypeId(typeInt->id());
    QString actual = m_Maker->signature(method);
    QString expect = "int getInt()";
    ASSERT_EQ(actual, expect);

    // const getter
    method->setConstStatus(true);
    actual = m_Maker->signature(method);
    expect = "int getInt() const";
    ASSERT_EQ(actual, expect);

    // pure virtual const getter
    method->addLhsIdentificator(entity::LhsIdentificator::Virtual);
    method->setRhsIdentificator(entity::RhsIdentificator::PureVirtual);
    actual = m_Maker->signature(method);
    expect = "virtual int getInt() const = 0";
    ASSERT_EQ(actual, expect);

    // with parameter
    method->addParameter("parameter1", typeInt->id());
    actual = m_Maker->signature(method);
    expect = "virtual int getInt(int parameter1) const = 0";
    ASSERT_EQ(actual, expect);

    // with parameters
    auto p2 = method->addParameter("parameter2", typeInt->id());
    actual = m_Maker->signature(method);
    expect = "virtual int getInt(int parameter1, int parameter2) const = 0";
    ASSERT_EQ(actual, expect);
}
