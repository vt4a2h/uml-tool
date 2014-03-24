#pragma once

#include "type.h"

namespace entity {

    class Field;

    class Union : public Type
    {
    public:
        Union();
        Union(const QString &name, Scope *scope);

    };

} // namespace entity
