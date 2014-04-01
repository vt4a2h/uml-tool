#include "templateclassmethod.h"
#include "constants.cpp"

namespace entity {

    TemplateClassMethod::TemplateClassMethod()
        : TemplateClassMethod(DEFAULT_NAME)
    {
    }

    TemplateClassMethod::TemplateClassMethod(const QString &name)
        : ClassMethod(name)
        , Template()
    {
    }

} // namespace entity
