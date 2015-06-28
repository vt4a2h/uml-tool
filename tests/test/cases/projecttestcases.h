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

#include "TestProject.h"

TEST_F(Project, LoadSaveProject)
{
    project_->database()->addScope("foo")->addType("bar");
    project_->save();

    EXPECT_TRUE(project_->isSaved())
            << "Project should be saved.";

    project::SharedProject newProject(std::make_shared<project::Project>("no name here ", "no path here"));
    newProject->load(rootPath_ + sep_ + project_->name().toLower().replace(" ", "_") + "." + PROJECT_FILE_EXTENTION);

    EXPECT_TRUE(newProject->isSaved())
            << "Project should be saved.";

    EXPECT_EQ(*project_, *newProject)
            << "Saved and loaded projects must be equal";
}
