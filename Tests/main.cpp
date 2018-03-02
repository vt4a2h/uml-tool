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
#include <QApplication>

#include <cases/ProjectMakerCases.h>
#include <cases/ProjectCases.h>
#include <cases/ProjectTranslatorCases.h>
#include <cases/RelationMakerCases.h>
#include <cases/DepthSearchCases.h>
#include <cases/FileMakerCases.h>
#include <cases/FileJsonCases.h>
#include <cases/TypeMakerTestCases.h>
#include <cases/IComponentsCases.h>
#include <cases/EntitiesCases.h>
#include <cases/ClassComponentsCases.h>
#include <cases/SignatureParserCases.h>
#include <cases/ComponentsMakerCases.h>
#include <cases/SignatureMakerCases.h>
#include <cases/CommandsCases.h>
#include <cases/ID.h>
#include <cases/HelpersCases.h>

#include "Arguments.hpp"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("uml-tool-tests");
    QApplication::setApplicationVersion("1.0");

    Testing::Arguments::instance().parse();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
