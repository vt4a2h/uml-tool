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

    QStringList AbstractProjectGenerator::errors() const
    {
        return m_ErrorList;
    }

    bool AbstractProjectGenerator::anyErrors() const
    {
        return !m_ErrorList.isEmpty();
    }

    bool AbstractProjectGenerator::valid() const
    {
        QFileInfo dir(m_OutputDirectory);

        if (!dir.isDir()) {
            m_ErrorList << QObject::tr("%1 is not a directory.").arg(m_OutputDirectory);
            return false;
        } else if (!dir.isWritable()) {
            m_ErrorList << QObject::tr("%1 is not writable.").arg(m_OutputDirectory);
            return false;
        }

        return true;
    }

} // namespace generator
