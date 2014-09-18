#include "abstractprojectgenerator.h"
#include <QFileInfo>

namespace generator {

    AbstractProjectGenerator::AbstractProjectGenerator()
    {
    }

    AbstractProjectGenerator::AbstractProjectGenerator(const db::SharedDatabase &globalDb,
                                                       const db::SharedDatabase &projectDb,
                                                       const QString &outputDirectory)
        : m_ProjectTranslator(globalDb, projectDb)
        , m_OutputDirectory(outputDirectory)
    {

    }

    AbstractProjectGenerator::~AbstractProjectGenerator()
    {
    }

    translator::ProjectTranslator AbstractProjectGenerator::projectTranslator() const
    {
        return m_ProjectTranslator;
    }

    translator::ProjectTranslator *AbstractProjectGenerator::projectTranslatorPtr()
    {
        return const_cast<translator::ProjectTranslator*>(projectTranslatorCPtr());
    }

    const translator::ProjectTranslator *AbstractProjectGenerator::projectTranslatorCPtr() const
    {
        return &m_ProjectTranslator;
    }

    void AbstractProjectGenerator::setProjectTranslator(const translator::ProjectTranslator &translator)
    {
        m_ProjectTranslator = translator;
    }

    QString AbstractProjectGenerator::outputDirectory() const
    {
        return m_OutputDirectory;
    }

    void AbstractProjectGenerator::setOutputDirectory(const QString &outputDirectory)
    {
        m_OutputDirectory = outputDirectory;
    }

    SharedErrorList AbstractProjectGenerator::errors() const
    {
        return m_ErrorList;
    }

    bool AbstractProjectGenerator::anyErrors() const
    {
        return !m_ErrorList->isEmpty();
    }

    AbstractProjectGenerator::GeneratorOptions AbstractProjectGenerator::options() const
    {
        return m_Options;
    }

    void AbstractProjectGenerator::setOptions(const GeneratorOptions &options)
    {
        m_Options = options;
    }

    QString AbstractProjectGenerator::projectName() const
    {
        return m_ProjectName;
    }

    void AbstractProjectGenerator::setProjectName(const QString &projectName)
    {
        m_ProjectName = projectName;
    }

    void AbstractProjectGenerator::generate()
    {
        doGenerate();
    }

} // namespace generator
