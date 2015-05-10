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

#include "abstractprojectgenerator.h"
#include "generator_types.hpp"

namespace generator {

    /**
     * @brief The Profile struct
     */
    struct Profile
    {
        QStringList headers;
        QStringList sources;

        ProfileVariables variables;
    };

    /**
     * @brief The BasicCppProjectGenerator class
     */
    class BasicCppProjectGenerator final : public AbstractProjectGenerator
    {
    public:
        BasicCppProjectGenerator();
        BasicCppProjectGenerator(const db::SharedDatabase &globalDb,
                                  const db::SharedDatabase &projectDb,
                                  const QString &outputDirectory = "");

    private:
        void doWrite() const override;
        void doGenerate() override;
        void generateFiles(const entity::SharedScope &scope, const SharedVirtualDirectory &directory);
        void addProfile();

        SharedVirtualDirectory m_RootOutputDirectory;
        Profile m_ProfileData;
    };

} // namespace generator
