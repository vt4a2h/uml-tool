#pragma once
#include "abstractprojectgenerator.h"

namespace generator {

    class BasicCppProjectGenerator : public AbstractProjectGenerator
    {
    public:
        BasicCppProjectGenerator();

        bool validate() const override;
        void generate() override;
    };

} // namespace generator
