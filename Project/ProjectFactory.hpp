/*****************************************************************************
**
** Copyright (C) 2019 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 6/1/2019.
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

#include <DB/DBTypes.hpp>

#include "ProjectTypes.hpp"

#include "QtHelpers.h"

namespace Projects {

    class ProjectFactory
    {
    public:
        NEITHER_COPIABLE_NOR_MOVABLE(ProjectFactory)

        ~ProjectFactory();

        static ProjectFactory &instance();

        void initialise(DB::SharedDatabase globalDb);

        DB::SharedDatabase globalDB() const;

        SharedProject makeProject() const;
        SharedProject makeProject(const QString &name, const QString &path) const;

    private:
        ProjectFactory();

        DB::SharedDatabase m_GlobalDB;
    };

} // namespace Projects
