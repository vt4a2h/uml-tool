#pragma once

#include "type.h"

namespace entity {

    class Class : public Type
    {
    public:
        enum Kind {ClassType, StructType};

        Class();
    };

} // namespace entity
