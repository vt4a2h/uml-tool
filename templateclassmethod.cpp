#include "templateclassmethod.h"

namespace entity {

    TemplateClassMethod::TemplateClassMethod()
        : TemplateClassMethod("noname")
    {
    }

    TemplateClassMethod::TemplateClassMethod(const QString &name)
        : ClassMethod(name)
        , Template()
    {
    }

} // namespace entity
