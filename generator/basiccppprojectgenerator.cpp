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

#include "basiccppprojectgenerator.h"

#include "virtualdirectory.h"
#include "enums.h"
#include "templates.cpp"

#include <QMap>

#include <db/database.h>
#include <db/ProjectDatabase.h>
#include <entity/scope.h>
#include <entity/type.h>
#include <entity/class.h>
#include <entity/enum.h>
#include <entity/union.h>
#include <entity/templateclass.h>
#include <entity/extendedtype.h>
#include <translation/code.h>

namespace generator {

    /**
     * @brief BasicCppProjectGenerator::BasicCppProjectGenerator
     */
    BasicCppProjectGenerator::BasicCppProjectGenerator()
        : AbstractProjectGenerator()
    {
    }

    /**
     * @brief BasicCppProjectGenerator::BasicCppProjectGenerator
     * @param globalDb
     * @param projectDb
     * @param outputDirectory
     */
    BasicCppProjectGenerator::BasicCppProjectGenerator(const db::SharedDatabase &globalDb,
                                                       const db::SharedProjectDatabase &projectDb,
                                                       const QString &outputDirectory)
        : AbstractProjectGenerator(globalDb, projectDb, outputDirectory)
        , m_RootOutputDirectory(std::make_shared<VirtualDirectory>(outputDirectory))
    {
        m_RootOutputDirectory->setErrorList(m_ErrorList);
        m_RootOutputDirectory->setPath(m_OutputDirectory);
    }

    /**
     * @brief BasicCppProjectGenerator::doWrite
     */
    void BasicCppProjectGenerator::doWrite() const
    {
        m_RootOutputDirectory->write();

        if (m_ErrorList && !m_ErrorList->isEmpty()) {
            m_RootOutputDirectory->clearVirtualStructure();
            m_RootOutputDirectory->addFile("errors.log")->setData(m_ErrorList->join("\n"));
        }
    }

    /**
     * @brief BasicCppProjectGenerator::doGenerate
     */
    void BasicCppProjectGenerator::doGenerate()
    {
        for (auto &&scope : m_ProjectTranslator.projectDatabase()->scopes())
            generateFiles(scope, m_RootOutputDirectory);

        addProfile();
    }

    /**
     * @brief BasicCppProjectGenerator::generateFiles
     * @param scope
     * @param directory
     */
    void BasicCppProjectGenerator::generateFiles(const entity::SharedScope &scope,
                                                 const SharedVirtualDirectory &directory)
    {
        SharedVirtualDirectory dir((m_Options & NamespacesInSubfolders) ?
                                       directory->addDirectory(scope->name().toLower()) : directory);

        for (auto &&t : scope->types()) {
            translation::Code code = m_ProjectTranslator.translate(t);
            m_ProjectTranslator.addNamespace(t, code);

            QString name(t->name().toLower());
            if (!code.toSource.isEmpty()) {
                  code.toSource.prepend(QString("#include \"%1.h\"\n\n").arg(name));
            }

            if (!code.toHeader.isEmpty()) {
                if (m_Options & DefineIcludeGuard) {
                    QString guardName = scope->name().toUpper() + "_" + t->name().toUpper() + "_H";
                    code.toHeader.prepend("#define "  + guardName + "\n\n");
                    code.toHeader.prepend("#ifndef "  + guardName + "\n");
                    code.toHeader.append("\n\n#endif // " + guardName);
                } else {
                    code.toHeader.prepend("#pragma once\n\n");
                }
            }

            auto addFile = [&](const QString &data, const QString &ext, QStringList &section) {
                QString fname(name + ext);
                dir->addFile(fname)->setData(data);
                section.append(fname);
            };

            if (!code.toHeader.isEmpty())
                addFile(code.toHeader, ".h", m_ProfileData.headers);
            if (!code.toSource.isEmpty())
                addFile(code.toSource, ".cpp", m_ProfileData.sources);

        }

        for (auto &&s : scope->scopes())
            generateFiles(s, dir);
    }

    /**
     * @brief BasicCppProjectGenerator::addProfile
     */
    void BasicCppProjectGenerator::addProfile()
    {
        // TODO: add different strateges for different project types
        QString name(m_ProjectName.remove(" ").toLower());
        name.append(".pro");

        auto file = m_RootOutputDirectory->addFile(name);

        // TODO: add more variables
        m_ProfileData.variables["HEADERS"] = m_ProfileData.headers.join(" \\\n" + INDENT).append("\n");
        m_ProfileData.variables["SOURCES"] = m_ProfileData.sources.join(" \\\n" + INDENT);

        for (auto &&key : m_ProfileData.variables.keys())
           file->appendData(QString("%1 %2 \\\n%3%4").arg(key,
                                                          "=",
                                                          INDENT,
                                                          m_ProfileData.variables[key]));
    }

} // namespace generator
