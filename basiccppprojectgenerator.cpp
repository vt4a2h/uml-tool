#include "basiccppprojectgenerator.h"
#include "virtualdirectory.h"
#include "database.h"
#include "scope.h"
#include "type.h"
#include "class.h"

namespace generator {

    BasicCppProjectGenerator::BasicCppProjectGenerator()
        : AbstractProjectGenerator()
    {
    }

    BasicCppProjectGenerator::BasicCppProjectGenerator(const db::SharedDatabase &globalDb,
                                                       const db::SharedDatabase &projectDb,
                                                       const QString &outputDirectory)
        : AbstractProjectGenerator(globalDb, projectDb, outputDirectory)
        , m_RootOutputDirectory(std::make_shared<VirtualDirectory>(outputDirectory))
    {
        m_RootOutputDirectory->setErrorList(m_ErrorList);
    }

    void BasicCppProjectGenerator::writeToDisk() const
    {
        m_RootOutputDirectory->write();

        if (!m_ErrorList->isEmpty()) {
            m_RootOutputDirectory->clearVirtualStructure();
            m_RootOutputDirectory->addFile("errors.log")->setData(m_ErrorList->join("\n"));
        }
    }

    void BasicCppProjectGenerator::doGenerate()
    {
        for (auto &&scope : m_ProjectTranslator.projectDatabase()->scopes())
            generateFiles(scope, m_RootOutputDirectory);

        addProfile();
    }

    void BasicCppProjectGenerator::generateFiles(const entity::SharedScope &scope,
                                                 const SharedVirtualDirectory &directory)
    {
        SharedVirtualDirectory dir(m_Options & NamespacesInSubfolders ?
                                       directory->addDirectory(scope->name().toLower()) : directory);

        for (auto &&t : scope->types()) {
            auto code(m_ProjectTranslator.generateCode(t));
            if (auto classPtr = std::dynamic_pointer_cast<entity::Class>(t))
                code.join(m_ProjectTranslator.generateClassMethodsImpl(classPtr));

            QString name(t->name().toLower());
            if (!code.isEmpty())
                code.toSource.prepend("\n").prepend(QString("#include \"%1\".h").arg(name));

            auto addFile = [&](const QString &data, const QString &ext, QStringList &section) {
                QString fname(name + ext);
                dir->addFile(fname)->setData(data);
                section.append(fname);
            };

            if (!code.toHeader.isEmpty())
                addFile(code.toHeader, "h", m_ProfileData.headers);
            if (!code.toSource.isEmpty())
                addFile(code.toSource, "cpp", m_ProfileData.sources);
        }

        for (auto &&s : scope->scopes())
            generateFiles(s, dir);
    }

    void BasicCppProjectGenerator::addProfile()
    {
        QString name(m_ProjectName.remove(" ").toLower());
        name.append(".pro");

        auto file = m_RootOutputDirectory->addFile(name);
        // NOTE: tmp output
        file->appendData(m_ProfileData.headers.join(", "), "")
             .appendData(m_ProfileData.sources.join(", "));
    }

} // namespace generator
