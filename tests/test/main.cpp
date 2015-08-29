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
#include <gtest/gtest.h>

#include <cases/projecttranslatortestcases.h>
#include <cases/relationmakertestcases.h>
#include <cases/depthsearchtestcases.h>
#include <cases/filemakertestcases.h>
#include <cases/filejsontestcases.h>
#include <cases/projectmakertestcases.h>
#include <cases/projecttestcases.h>
#include <cases/typemakertestcases.h>
#include <cases/icomponentstestcases.h>
#include <cases/entitiestestcases.h>
#include <cases/signatureparsercases.h>
#include <cases/componentsmakercases.h>
#include <cases/signaturemakercases.h>
#include <cases/classcomponentscases.h>

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
