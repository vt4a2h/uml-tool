/*****************************************************************************
**
** Copyright (C) 2018 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 28 .
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
#include "Arguments.hpp"

#include <QCommandLineParser>

namespace Testing
{

    Arguments::Arguments()
    {
    }

    QString Arguments::rootPath() const
    {
        return m_rootPath;
    }

    Arguments & Arguments::instance()
    {
        static Arguments args;
        return args;
    }

    void Arguments::parse()
    {
        QCommandLineParser parser;
        parser.setApplicationDescription("Unit tests");
        parser.addHelpOption();

        static const QString rootOptionName = "r";
        static const QString dbOptionName   = "d";
        parser.addOptions({
            {{rootOptionName, "test_root"}, "Set tests root path. Have to be specified"   , "root"},
            {{dbOptionName  , "db_path"  }, "Set main database path. Have to be specified", "db"  },
            {"gtest_shuffle", "Shuffle tests"},
        });

        if (!parser.parse(qApp->arguments()))
            parser.showHelp();

        auto tmpRootPath = parser.value(rootOptionName);
        auto tmpDbPath   = parser.value(dbOptionName);

        if (tmpRootPath.isEmpty() || tmpDbPath.isEmpty())
            parser.showHelp();

        m_rootPath = tmpRootPath;
        m_dbPath   = tmpDbPath;
    }

    QString Arguments::dbPath() const
    {
        return m_dbPath;
    }

} // namespace Testing
