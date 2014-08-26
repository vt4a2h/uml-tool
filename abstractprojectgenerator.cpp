#include "abstractprojectgenerator.h"

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

    QStringList AbstractProjectGenerator::errors() const
    {
        return m_ErrorList;
    }

    bool AbstractProjectGenerator::anyErrors() const
    {
        return !m_ErrorList.isEmpty();
    }

} // namespace generator
