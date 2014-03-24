#include "union.h"
#include "field.h"

namespace entity {

    Union::Union()
        : Union("noname", nullptr)
    {
    }

    Union::Union(const QString &name, Scope *scope)
        : Type(name, scope)
    {
    }

} // namespace entity
