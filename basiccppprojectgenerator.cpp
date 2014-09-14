#include "basiccppprojectgenerator.h"
#include "virtualdirectory.h"

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

    void BasicCppProjectGenerator::generate()
    {
    }

} // namespace generator
