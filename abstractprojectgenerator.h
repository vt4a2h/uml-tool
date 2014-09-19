#pragma once
#include "projecttranslator.h"

namespace generator {

    class AbstractProjectGenerator
    {
    public:
        enum GeneratorOption {
            NoOptions = 0x0,
            NamespacesInSubfolders = 0x1
        };
        Q_DECLARE_FLAGS(GeneratorOptions, GeneratorOption)

       AbstractProjectGenerator();
       AbstractProjectGenerator(const db::SharedDatabase &globalDb,
                                const db::SharedDatabase &projectDb,
                                const QString &outputDirectory = "");
       virtual ~AbstractProjectGenerator();

       translator::ProjectTranslator projectTranslator() const;
       translator::ProjectTranslator *projectTranslatorPtr();
       const translator::ProjectTranslator *projectTranslatorCPtr() const;
       void setProjectTranslator(const translator::ProjectTranslator &translator);

       QString outputDirectory() const;
       void setOutputDirectory(const QString &outputDirectory);

       SharedErrorList errors() const;
       bool anyErrors() const;

       GeneratorOptions options() const;
       void setOptions(const GeneratorOptions &options);

       QString projectName() const;
       void setProjectName(const QString &projectName);

       void generate();
       void writeToDisk() const;

    protected:
       virtual void doGenerate() = 0;
       virtual void doWrite() const = 0;

       translator::ProjectTranslator m_ProjectTranslator;
       GeneratorOptions m_Options;
       QString m_OutputDirectory;
       QString m_ProjectName;
       mutable SharedErrorList m_ErrorList;
    };

    Q_DECLARE_OPERATORS_FOR_FLAGS(AbstractProjectGenerator::GeneratorOptions)

} // namespace generator
