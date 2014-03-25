#include "union.h"
#include "field.h"
#include "extendedtype.h"

namespace entity {

    Union::Union()
        : Union("noname", nullptr)
    {
    }

    Union::Union(const QString &name, Scope *scope)
        : Type(name, scope)
    {
    }

    SharedField Union::getField(const QString &name)
    {
        SharedField result = nullptr;

        auto it = m_Fields.find(name);
        if (it != m_Fields.end()) result =  *it;

        return result;
    }

    SharedField Union::addField(const QString &name, ExtendedType *type)
    {
        return *m_Fields.insert(name, std::make_shared<Field>(name, type));
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
