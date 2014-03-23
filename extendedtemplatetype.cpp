#include "extendedtemplatetype.h"
#include "type.h"

#include <algorithm>

namespace entity {

    ExtendedTemplateType::ExtendedTemplateType()
        : ExtendedType()
    {
    }

    ExtendedTemplateType::ExtendedTemplateType(Scope *scope, Type *type)
        : ExtendedType(scope, type)
    {
    }

    ExtendedType *ExtendedTemplateType::getTemplateParameter(const QString &typeAliasOrName) const
    {
        ExtendedType *result(nullptr);

        auto it = std::find_if(m_TemplateParameters.begin(), m_TemplateParameters.end(),
                               [&](ExtendedType *t){ return t->alias() == typeAliasOrName; });
        if (it != m_TemplateParameters.end()) {
            result = *it;
        }
        else {
            it = std::find_if(m_TemplateParameters.begin(), m_TemplateParameters.end(),
                              [&](ExtendedType *t){ return t->type()->name() == typeAliasOrName; });
            if (it != m_TemplateParameters.end()) result = *it;
        }

        return result;
    }

    ExtendedType::ExtendedTypes ExtendedTemplateType::parameters() const
    {
        return m_TemplateParameters;
    }

    void ExtendedTemplateType::addTemplateParameter(ExtendedType *parameter)
    {
        m_TemplateParameters.append(parameter);
    }

    bool ExtendedTemplateType::contains(ExtendedType *type) const
    {
        return m_TemplateParameters.contains(type);
    }

    bool ExtendedTemplateType::contains(const QString &typeAliasOrName) const
    {
        return getTemplateParameter(typeAliasOrName);
    }

    void ExtendedTemplateType::removeParameter(ExtendedType *type)
    {
        auto it = std::find(m_TemplateParameters.begin(), m_TemplateParameters.end(),
                            type);
        if (it != m_TemplateParameters.end()) m_TemplateParameters.erase(it);
    }

} // namespace entity
