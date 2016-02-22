/*****************************************************************************
**
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 29/10/2014.
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

#include <QHash>
#include <QString>

#include <gtest/gtest.h>

#include <entity/type.h>
#include <entity/class.h>
#include <entity/enum.h>
#include <entity/union.h>
#include <db/database.h>
#include <entity/scope.h>

#include "TestProjectBase.h"

class DepthSearch : public ProjectBase
{
protected:
    virtual void SetUp() override
    {
        _scopes["sc1"] = m_ProjectDb->addScope("sc1"); // sc1::sc2::sc3
        _scopes["sc2"] = _scopes["sc1"]->addChildScope("sc2");
        _scopes["sc3"] = _scopes["sc2"]->addChildScope("sc3");

        _scopes["sc4"] = m_ProjectDb->addScope("sc4");
        _scopes["sc5"] = _scopes["sc4"]->addChildScope("sc5");

        _types["Foo"] = _scopes["sc1"]->addType("Foo");
        _types["Bar"] = _scopes["sc2"]->addType<entity::Class>("Bar");
        _types["Baz"] = _scopes["sc3"]->addType<entity::Enum>("Baz");
        _types["FooBar"] = _scopes["sc4"]->addType<entity::Union>("FooBar");
    }

    QHash<QString, entity::SharedScope> _scopes;
    QHash<QString, entity::SharedType> _types;
};
