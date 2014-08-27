#include "basiccppprojectgenerator.h"

namespace generator {

    BasicCppProjectGenerator::BasicCppProjectGenerator()
    {
    }

    bool BasicCppProjectGenerator::valid() const
    {
        return AbstractProjectGenerator::valid();
    }

    void BasicCppProjectGenerator::generate()
    {
        if (!valid()) return;

        // NOTE: stub
    }

} // namespace generator
