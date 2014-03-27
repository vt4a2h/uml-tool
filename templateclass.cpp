#include "templateclass.h"

namespace entity {

    TemplateClass::TemplateClass()
        : TemplateClass("noname", nullptr)
    {
    }

    TemplateClass::TemplateClass(const QString &name, Scope *scope)
        : Class (name, scope)
        , Template()
    {
    }

} // namespace entity
