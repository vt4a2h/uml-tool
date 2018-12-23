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

#include "Tests/TestFileMaker.h"

TEST_F(FileMaker, MakeFileStructure)
{
    m_Directory.setPath(m_RootPath + m_Sep + "dir_name");
    m_Directory.write();
    QFileInfo info(m_Directory.path());

    EXPECT_TRUE(m_Directory.errorList()->isEmpty())
            << "Directory should be created without any errors";
    EXPECT_TRUE(info.isDir())
            << "VirtualDirectory::write should create directory.";
    EXPECT_TRUE(info.isWritable())
            << "Directory should be writable.";

    Generator::SharedVirtualDirectory dir1(m_Directory.addDirectory("dir1"));

    EXPECT_TRUE(m_Directory.constainsDirectory("dir1"))
            << "Directory should contains nested directory.";
    EXPECT_FALSE(m_Directory.constainsDirectory("dir123"))
            << "Directory should not contains wrong nested directory.";

    m_Directory.write();
    info = dir1->path();

    EXPECT_TRUE(m_Directory.errorList()->isEmpty())
            << "Nested directory should be created without any errors";
    EXPECT_TRUE(info.isDir())
            << "VirtualDirectory::write should create nested directory.";
    EXPECT_TRUE(info.isWritable())
            << "Nested directory should be writable.";

    Generator::SharedVirtualFile file1(m_Directory.addFile("foo.cpp"));

    EXPECT_TRUE(m_Directory.constainsFile("foo.cpp"))
            << "Directory should contains nested file.";
    EXPECT_FALSE(m_Directory.constainsFile("dir1"))
            << "Directory should not contains wrong nested file.";

    QString fileData("int main()\n{\n}");
    file1->setData(fileData);
    m_Directory.write();
    info = file1->path();

    EXPECT_TRUE(m_Directory.errorList()->isEmpty())
            << "File should be created without any errors";
    EXPECT_TRUE(info.isFile())
            << "VirtualFile::write should create a file.";
    EXPECT_TRUE(info.isReadable())
            << "File should be readable.";

    QFile f(file1->path());
    EXPECT_TRUE(f.open(QIODevice::ReadOnly | QIODevice::Text))
            << "File should be openable";
    QTextStream s(&f);
    EXPECT_EQ(fileData, s.readAll())
            << "File should contains right data";
    f.close();
}

