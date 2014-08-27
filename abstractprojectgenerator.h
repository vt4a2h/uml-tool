#pragma once
#include "projecttranslator.h"

namespace generator {

    class AbstractProjectGenerator
    {
    public:
        enum GeneratorOption {};
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

       QStringList errors() const;
       bool anyErrors() const;

       virtual bool valid() const;
       virtual void generate() = 0;

    protected:
       translator::ProjectTranslator m_ProjectTranslator;
       QString m_OutputDirectory;
       mutable QStringList m_ErrorList;
    };

    Q_DECLARE_OPERATORS_FOR_FLAGS(AbstractProjectGenerator::GeneratorOptions)

} // namespace generator
