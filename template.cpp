#include "template.h"
#include "extendedtype.h"
#include "type.h"
#include "constants.cpp"

#include <algorithm>
#include <utility>

namespace entity {

    TemplateParameter Template::getTemplateParameter(const QString &name) const
    {
        auto it = m_TemplateParameters.find(name);
        return (it != m_TemplateParameters.end() ? *it : std::make_pair(DEFAULT_NAME, STUB_ID));
    }

    void Template::addTemplateParameter(const QString &name, const QString &typeId)
    {
        m_TemplateParameters.insert(name, std::make_pair(name, typeId.isEmpty() ? STUB_ID : typeId));
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
