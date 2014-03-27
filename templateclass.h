#pragma once

#include "class.h"
#include "template.h"

namespace entity {

    class TemplateClass : public Class, public Template
    {
    public:
        TemplateClass();
        TemplateClass(const QString &name, Scope *scope);
    };

} // namespace entity
