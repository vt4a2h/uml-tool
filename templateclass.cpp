#include "templateclass.h"
#include "constants.cpp"
#include "enums.h"

namespace entity {

    TemplateClass::TemplateClass()
        : TemplateClass(DEFAULT_NAME, GLOBAL_SCOPE_ID)
    {
        m_KindOfType = TemplateClassType;
    }

    TemplateClass::TemplateClass(const QString &name, const QString &scopeId)
        : Class (name, scopeId)
        , Template()
    {
    }

} // namespace entity
