#pragma once
#include "abstractprojectgenerator.h"

namespace generator {

    class BasicCppProjectGenerator : public AbstractProjectGenerator
    {
    public:
        BasicCppProjectGenerator();

        bool valid() const override;
        void generate() override;
    };

} // namespace generator
