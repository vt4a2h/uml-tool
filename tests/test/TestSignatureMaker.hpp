/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 12/08/2015.
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

#include <gtest/gtest.h>

#include "TestComponentsBase.h"

#include <translation/signaturemaker.h>
#include <translation/translator_types.hpp>

class SignatureMaker : public ComponentsBase, public ::testing::Test
{
protected:
    SignatureMaker()
    : m_Maker(std::make_unique<translation::SignatureMaker>(m_ApplicationModel->globalDatabase(),
                                                            m_Project->database(), m_Scope, m_Type))
    {}

protected:
   translation::UniqueSignatureMaker m_Maker;
};
