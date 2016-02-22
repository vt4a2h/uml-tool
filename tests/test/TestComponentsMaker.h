/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 13/07/2015.
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

#include <entity/components/componentsmaker.h>
#include <entity/components/componentsignatureparser.h>

#include "TestComponentsBase.h"

class ComponentsMaker : public ComponentsBase, public ::testing::Test
{
protected:
    ComponentsMaker()
        : m_Maker(std::make_unique<components::ComponentsMaker>(m_ApplicationModel, m_Type, m_ProjectScope))
        , m_Parser(std::make_unique<components::ComponentSignatureParser>())
    {}

    components::OptionalEntity parseAndMake(const QString &in, models::DisplayPart part)
    {
        m_Parser->parse(in, part);
        return m_Maker->makeComponent(m_Parser->tokens(), part);
    }

protected:
    components::UniqueComponentsMaker m_Maker;
    components::UniqueSignatureParser m_Parser;
};

