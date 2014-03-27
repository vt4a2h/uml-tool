#pragma once

#include "classmethod.h"
#include "template.h"

namespace entity {

    class TemplateClassMethod : public ClassMethod, public Template
    {
    public:
        TemplateClassMethod();
        TemplateClassMethod(const QString &name);
    };

} // namespace entity
