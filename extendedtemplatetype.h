#pragma once

#include "extendedtype.h"

namespace entity {

    class ExtendedTemplateType : public ExtendedType
    {
    public:
        ExtendedTemplateType();
        ExtendedTemplateType(Scope *scope, Type *type);

        ExtendedType *getTemplateParameter(const QString &typeAliasOrName) const;
        void addTemplateParameter(ExtendedType *parameter);
        bool contains(ExtendedType *type) const;
        bool contains(const QString &typeAliasOrName) const;
        void removeParameter(ExtendedType *type);

    private:
        ExtendedTypes m_TemplateParameters;
    };

} // namespace entity

