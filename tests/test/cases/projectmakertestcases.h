/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 28/06/2015.
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

#include "TestProjectMaker.h"
#include "helpers.h"

TEST_F(ProjectMaker, MakeClass)
{
    auto scopeWork = projectDb_->addScope("work");
    auto empClass  = scopeWork->addType<entity::Class>("Employee");

    empClass->addField("firstName", string_->id(), "m_", entity::Private);
    empClass->addField("lastName",  string_->id(), "m_", entity::Private);
    empClass->addField("status",    bool_->id(),   "m_", entity::Private);
    empClass->addField("salary",    double_->id(), "m_", entity::Private);

    auto constLinkToString = globalScope_->addType<entity::ExtendedType>();
    constLinkToString->setTypeId(string_->id());
    constLinkToString->setConstStatus(true);
    constLinkToString->addLinkStatus();

    empClass->makeMethod("Employee")->setReturnTypeId(STUB_ID);

    auto parCtor = empClass->makeMethod("Employee");
    parCtor->setReturnTypeId(STUB_ID);
    parCtor->addParameter("firstName", constLinkToString->id());
    parCtor->addParameter("lastName", constLinkToString->id());

    auto firstNameGetter = empClass->makeMethod("firstName");
    firstNameGetter->setReturnTypeId(string_->id());
    firstNameGetter->setConstStatus(true);
    empClass->makeMethod("setFirstName")->addParameter("firstName", constLinkToString->id());

    auto lastNameGetter = empClass->makeMethod("lastName");
    lastNameGetter->setReturnTypeId(string_->id());
    lastNameGetter->setConstStatus(true);
    empClass->makeMethod("setLastName")->addParameter("lastName", constLinkToString->id());

    auto isHired = empClass->makeMethod("isHired");
    isHired->setReturnTypeId(bool_->id());
    isHired->setConstStatus(true);
    empClass->makeMethod("fire")->setReturnTypeId(bool_->id());
    empClass->makeMethod("hire")->setReturnTypeId(bool_->id());

    auto salaryGetter = empClass->makeMethod("salary");
    salaryGetter->setReturnTypeId(double_->id());
    salaryGetter->setConstStatus(true);
    empClass->makeMethod("setSalary")->addParameter("salary", double_->id());

    generator_->generate();
    generator_->writeToDisk();

    read_from(tstHeader, fTstHeader, testDataPath_ + empClass->name().toLower() + ".h")
    read_from(genHeader, fGenHeader, rootPath_ + sep_ + empClass->name().toLower() + ".h")
    EXPECT_EQ(tstHeader.toStdString(), genHeader.toStdString())
            << "Generated data for header must be the same with test data";

    read_from(tstSource, fTstSource, testDataPath_ + empClass->name().toLower() + ".cpp")
    read_from(genSource, fGenSource, rootPath_ +  sep_ + empClass->name().toLower() + ".cpp")
    EXPECT_EQ(tstSource.toStdString(), genSource.toStdString())
            << "Generated data for source must be the same with test data";
}

TEST_F(ProjectMaker, MakeTemplateClass)
{
    auto scopeMemory = projectDb_->addScope("memory");
    auto ptrClass    = scopeMemory->addType<entity::TemplateClass>("shared_pointer");
    auto linkToPtr   = scopeMemory->addType<entity::ExtendedType>();
    linkToPtr->setTypeId(ptrClass->id());
    linkToPtr->addLinkStatus();

    auto typeT       = ptrClass->addLocalType("Value");
    ptrClass->addTemplateParameter(typeT->id());

    auto pointerToT = ptrClass->addLocalType<entity::ExtendedType>();
    pointerToT->setTypeId(typeT->id());
    pointerToT->addPointerStatus();

    auto cPointerToT = ptrClass->addLocalType<entity::ExtendedType>();
    cPointerToT->setTypeId(typeT->id());
    cPointerToT->addPointerStatus();
    cPointerToT->setConstStatus(true);

    auto ctor = ptrClass->makeMethod("shared_pointer");
    ctor->setReturnTypeId(STUB_ID);
    ctor->addLhsIdentificator(entity::Explicit);
    ctor->addParameter("value", pointerToT->id())->setDefaultValue("nullptr");

    ptrClass->makeMethod("~shared_pointer")->setReturnTypeId(STUB_ID);

    auto resetMethod = ptrClass->makeMethod("reset");
    resetMethod->addParameter("other", pointerToT->id())->setDefaultValue("nullptr");

    auto useCountMethod = ptrClass->makeMethod("use_count");
    useCountMethod->setReturnTypeId(uint_->id());
    useCountMethod->setConstStatus(true);

    auto getter = ptrClass->makeMethod("get");
    getter->setReturnTypeId(cPointerToT->id());
    getter->setConstStatus(true);

    ptrClass->makeMethod("get")->setReturnTypeId(pointerToT->id());

    auto swapMethod = ptrClass->makeMethod("swap");
    swapMethod->addParameter("src", linkToPtr->id());
    swapMethod->setSection(entity::Private);

    generator_->generate();
    generator_->writeToDisk();

    read_from(tstHeader, fTstHeader, testDataPath_ + ptrClass->name().toLower() + ".h")
    read_from(genHeader, fGenHeader, rootPath_ + sep_ + ptrClass->name().toLower() + ".h")
    EXPECT_EQ(tstHeader, genHeader)
            << "Generated data for header must be the same with test data";
}
