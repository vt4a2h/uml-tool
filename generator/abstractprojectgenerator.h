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

#include <translation/projecttranslator.h>

#include "types.h"

/**
 * @brief  generator
 */
namespace generator {

    /**
     * @brief The AbstractProjectGenerator class
     */
    class AbstractProjectGenerator
    {
    public:
        /**
         * @brief The GeneratorOption enum
         */
        enum GeneratorOption {
            NoOptions = 0x0,
            NamespacesInSubfolders = 0x1,
            DefineIcludeGuard = 0x2, // pragma by default
        };
        Q_DECLARE_FLAGS(GeneratorOptions, GeneratorOption)

       AbstractProjectGenerator();
       AbstractProjectGenerator(const db::SharedDatabase &globalDb,
                                const db::SharedDatabase &projectDb,
                                const QString &outputDirectory = "");
       virtual ~AbstractProjectGenerator();

       translation::ProjectTranslator projectTranslator() const;
       translation::ProjectTranslator *projectTranslatorPtr();
       const translation::ProjectTranslator *projectTranslatorCPtr() const;
       void setProjectTranslator(const translation::ProjectTranslator &translator);

       QString outputDirectory() const;
       void setOutputDirectory(const QString &outputDirectory);

       SharedErrorList errors() const;
       bool anyErrors() const;

       GeneratorOptions options() const;
       void setOptions(const GeneratorOptions &options);
       void addOption(GeneratorOption option);

       QString projectName() const;
       void setProjectName(const QString &projectName);

       void generate();
       void writeToDisk() const;

    protected:
       virtual void doGenerate() = 0;
       virtual void doWrite() const = 0;

       translation::ProjectTranslator m_ProjectTranslator;
       GeneratorOptions m_Options;
       QString m_OutputDirectory;
       QString m_ProjectName;
       mutable SharedErrorList m_ErrorList;
    };

    Q_DECLARE_OPERATORS_FOR_FLAGS(AbstractProjectGenerator::GeneratorOptions)

} // namespace generator
