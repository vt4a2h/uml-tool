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
#include "Constants.h"

TEST_F(TestProjects, LoadSaveProject)
{
    m_Project->database()->addScope("foo")->addType("bar");
    m_Project->save();

    EXPECT_FALSE(m_Project->isModified())
        << "Project should be saved.";

    auto oldProject = m_Project;

    setProject(std::make_shared<Projects::Project>("no name here ", "no path here"));
    m_Project->load(rootPath_ + sep_ + oldProject->name().toLower().replace(" ", "_") +
                    "." + PROJECT_FILE_EXTENTION);

    EXPECT_FALSE(m_Project->isModified())
            << "Project should be saved.";

    // We should increase id for old project, because it was increased twice in the loaded project
    // once for "foo" and once for "bar"
    oldProject->genID();
    oldProject->genID();

    EXPECT_EQ(*oldProject, *m_Project)
            << "Saved and loaded projects must be equal";
}
