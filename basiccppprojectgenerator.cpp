#include "basiccppprojectgenerator.h"
#include "virtualdirectory.h"
#include "database.h"
#include "enums.h"
#include "scope.h"
#include "type.h"
#include "class.h"
#include "enum.h"
#include "union.h"
#include "templateclass.h"
#include "extendedtype.h"
#include "code.h"

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
        m_RootOutputDirectory->setPath(m_OutputDirectory);
    }

    void BasicCppProjectGenerator::doWrite() const
    {
        m_RootOutputDirectory->write();

        if (m_ErrorList && !m_ErrorList->isEmpty()) {
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
            // temporary solution
            translator::Code code;
            switch (t->type()) {
            case entity::BasicType:
                code = m_ProjectTranslator.translate(t);
                break;
            case entity::EnumType:
                code = m_ProjectTranslator.translate(std::static_pointer_cast<entity::Enum>(t));
                break;
            case entity::ExtendedTypeType:
                code = m_ProjectTranslator.translate(std::static_pointer_cast<entity::ExtendedType>(t));
                break;
            case entity::TemplateClassType:
                code = m_ProjectTranslator.translate(std::static_pointer_cast<entity::TemplateClass>(t));
                break;
            case entity::UnionType:
                code = m_ProjectTranslator.translate(std::static_pointer_cast<entity::Union>(t));
                break;
            case entity::UserClassType:
                code = m_ProjectTranslator.translate(std::static_pointer_cast<entity::Class>(t));
                break;
            default:
                code = m_ProjectTranslator.translate(t);
                break;
            }
            // end of temporary solution

            // TODO: delete after refactoring
            if (auto classPtr = std::dynamic_pointer_cast<entity::Class>(t))
                code.join(m_ProjectTranslator.generateClassMethodsImpl(classPtr));

            QString name(t->name().toLower());
            if (!code.toSource.isEmpty())
                code.toSource.prepend("\n").prepend(QString("#include \"%1.h\"").arg(name));

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
