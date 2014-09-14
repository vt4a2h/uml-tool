#pragma once
#include "abstractprojectgenerator.h"
#include "types.h"

namespace generator {

    class BasicCppProjectGenerator final : public AbstractProjectGenerator
    {
    public:
        BasicCppProjectGenerator();
        BasicCppProjectGenerator(const db::SharedDatabase &globalDb,
                                  const db::SharedDatabase &projectDb,
                                  const QString &outputDirectory = "");

        void generate() override;

    private:
        SharedVirtualDirectory m_RootOutputDirectory;
    };

} // namespace generator
