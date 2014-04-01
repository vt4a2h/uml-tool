#include "union.h"
#include "field.h"
#include "extendedtype.h"
#include "constants.cpp"

namespace entity {

    Union::Union()
        : Union(DEFAULT_NAME, GLOBAL_SCOPE_ID)
    {
    }

    Union::Union(const QString &name, const QString &scopeId)
        : Type(name, scopeId)
    {
    }

    SharedField Union::getField(const QString &name)
    {
        return m_Fields.value(name);
    }

    SharedField Union::addField(const QString &name, const QString &typeId)
    {
        return *m_Fields.insert(name, std::make_shared<Field>(name, typeId));
    }

    void Union::removeField(const QString &name)
    {
        m_Fields.remove(name);
    }

    bool Union::containsField(const QString &name)
    {
        return m_Fields.contains(name);
    }

    FieldsList Union::fields() const
    {
        return m_Fields.values();
    }

} // namespace entity
