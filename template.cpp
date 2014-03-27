#include "template.h"
#include "extendedtype.h"
#include "type.h"

#include <algorithm>
#include <utility>

namespace entity {

    TemplateParameter Template::getTemplateParameter(const QString &name) const
    {
        auto it = m_TemplateParameters.find(name);
        return (it != m_TemplateParameters.end() ? *it : std::make_pair("", nullptr));
    }

    void Template::addTemplateParameter(const QString &name, ExtendedType *t)
    {
        m_TemplateParameters.insert(name, std::make_pair(name, t));
    }

    bool Template::contains(const QString &name) const
    {
        return m_TemplateParameters.contains(name);
    }

    void Template::removeParameter(const QString &name)
    {
        m_TemplateParameters.remove(name);
    }

    TemplateParametersList Template::parameters() const
    {
        return m_TemplateParameters.values();
    }

} // namespace entity
