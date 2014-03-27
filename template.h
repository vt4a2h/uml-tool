#pragma once

#include "types.h"

namespace entity {

    class Template
    {
    public:
        TemplateParameter getTemplateParameter(const QString &name) const;
        void addTemplateParameter(const QString &name, ExtendedType *t = nullptr);
        bool contains(const QString &name) const;
        void removeParameter(const QString &name);
        TemplateParametersList parameters() const;

    private:
        TemplateParameters m_TemplateParameters;
    };

} // namespace entity

