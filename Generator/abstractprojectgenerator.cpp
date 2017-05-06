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

#include "abstractprojectgenerator.h"

#include <QFileInfo>
#include <QDebug>

namespace Generator {

    /**
     * @brief AbstractProjectGenerator::AbstractProjectGenerator
     */
    AbstractProjectGenerator::AbstractProjectGenerator()
        : AbstractProjectGenerator(nullptr, nullptr, "")
    {
        qWarning() << "Project generator created with empty databases and (or) with empty output path";
    }

    /**
     * @brief AbstractProjectGenerator::AbstractProjectGenerator
     * @param globalDb
     * @param projectDb
     * @param outputDirectory
     */
    AbstractProjectGenerator::AbstractProjectGenerator(const DB::SharedDatabase &globalDb,
                                                       const DB::SharedProjectDatabase &projectDb,
                                                       const QString &outputDirectory)
        : m_ProjectTranslator(globalDb, projectDb)
        , m_OutputDirectory(outputDirectory)
        , m_ErrorList(std::make_shared<ErrorList>())
    {

    }

    /**
     * @brief AbstractProjectGenerator::~AbstractProjectGenerator
     */
    AbstractProjectGenerator::~AbstractProjectGenerator()
    {
    }

    /**
     * @brief AbstractProjectGenerator::projectTranslator
     * @return
     */
    Translation::ProjectTranslator AbstractProjectGenerator::projectTranslator() const
    {
        return m_ProjectTranslator;
    }

    /**
     * @brief AbstractProjectGenerator::projectTranslatorPtr
     * @return
     */
    Translation::ProjectTranslator *AbstractProjectGenerator::projectTranslatorPtr()
    {
        return const_cast<Translation::ProjectTranslator*>(projectTranslatorCPtr());
    }

    /**
     * @brief AbstractProjectGenerator::projectTranslatorCPtr
     * @return
     */
    const Translation::ProjectTranslator *AbstractProjectGenerator::projectTranslatorCPtr() const
    {
        return &m_ProjectTranslator;
    }

    /**
     * @brief AbstractProjectGenerator::setProjectTranslator
     * @param translator
     */
    void AbstractProjectGenerator::setProjectTranslator(const Translation::ProjectTranslator &translator)
    {
        m_ProjectTranslator = translator;
    }

    /**
     * @brief AbstractProjectGenerator::outputDirectory
     * @return
     */
    QString AbstractProjectGenerator::outputDirectory() const
    {
        return m_OutputDirectory;
    }

    /**
     * @brief AbstractProjectGenerator::setOutputDirectory
     * @param outputDirectory
     */
    void AbstractProjectGenerator::setOutputDirectory(const QString &outputDirectory)
    {
        m_OutputDirectory = outputDirectory;
    }

    /**
     * @brief AbstractProjectGenerator::errors
     * @return
     */
    SharedErrorList AbstractProjectGenerator::errors() const
    {
        return m_ErrorList;
    }

    /**
     * @brief AbstractProjectGenerator::anyErrors
     * @return
     */
    bool AbstractProjectGenerator::anyErrors() const
    {
        return !m_ErrorList->isEmpty();
    }

    /**
     * @brief AbstractProjectGenerator::options
     * @return
     */
    AbstractProjectGenerator::GeneratorOptions AbstractProjectGenerator::options() const
    {
        return m_Options;
    }

    /**
     * @brief AbstractProjectGenerator::setOptions
     * @param options
     */
    void AbstractProjectGenerator::setOptions(const GeneratorOptions &options)
    {
        m_Options = options;
    }

    /**
     * @brief AbstractProjectGenerator::addOption
     * @param option
     */
    void AbstractProjectGenerator::addOption(AbstractProjectGenerator::GeneratorOption option)
    {
        m_Options |= option;
    }

    /**
     * @brief AbstractProjectGenerator::projectName
     * @return
     */
    QString AbstractProjectGenerator::projectName() const
    {
        return m_ProjectName;
    }

    /**
     * @brief AbstractProjectGenerator::setProjectName
     * @param projectName
     */
    void AbstractProjectGenerator::setProjectName(const QString &projectName)
    {
        m_ProjectName = projectName;
    }

    /**
     * @brief AbstractProjectGenerator::generate
     */
    void AbstractProjectGenerator::generate()
    {
        doGenerate();
    }

    /**
     * @brief AbstractProjectGenerator::writeToDisk
     */
    void AbstractProjectGenerator::writeToDisk() const
    {
        doWrite();
    }

} // namespace generator
