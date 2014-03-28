#include "scope.h"
#include "type.h"
#include "class.h"
#include "templateclass.h"
#include "union.h"
#include "enum.h"
#include "enums.h"
#include "extendedtype.h"
#include <utility>

namespace entity {

    Scope::Scope(const QString &scopeName)
        : m_Name(scopeName)
    {
    }

    QString Scope::name() const
    {
        return m_Name;
    }

    void Scope::setName(const QString &name)
    {
        m_Name = name;
    }

    Scope *Scope::parentScope() const
    {
        return m_ParentScope;
    }

    void Scope::setParentScope(Scope *parentScope)
    {
        m_ParentScope = parentScope;
    }

    SharedType Scope::getType(const QString &name) const
    {
        return (m_Types.contains(name) ? m_Types[name] : nullptr);
    }

    SharedType Scope::addType(const QString &name, UserType kindOfType)
    {
        switch (kindOfType) {
            case BasicType:
                return *m_Types.insert(name, std::make_shared<Type>(name, this));
            case UserClassType:
                return *m_Types.insert(name, std::make_shared<Class>(name, this));
            case TemplateClassType:
                return *m_Types.insert(name, std::make_shared<TemplateClass>(name, this));
            case UnionType:
                return *m_Types.insert(name, std::make_shared<Union>(name, this));
            case EnumType:
                return *m_Types.insert(name, std::make_shared<Enum>(name, this));
        }
        return nullptr;
    }

    bool Scope::containsType(const QString &name) const
    {
        return m_Types.contains(name);
    }

    void Scope::removeType(const QString &name)
    {
        m_Types.remove(name);
    }

    TypesList Scope::types() const
    {
        return m_Types.values();
    }

    SharedExtendedType Scope::getExtendedType(const QString &alias, SharedType type) const
    {
        QString key(QString("%1-%2").arg(alias, type->name()));
        return (m_ExtendedType.contains(key) ? m_ExtendedType[key] : nullptr);
    }

    SharedExtendedType Scope::addExtendedType(const QString &alias, SharedType type)
    {
        return *m_ExtendedType.insert(QString("%1-%2").arg(alias, type->name()),
                                      std::make_shared<ExtendedType>(this, type.get(), alias));
    }

    bool Scope::containsExtendedType(const QString &alias, SharedType type) const
    {
        return m_ExtendedType.contains(QString("%1-%2").arg(alias, type->name()));
    }

    void Scope::removeExtendedType(const QString &alias, SharedType type)
    {
        m_ExtendedType.remove(QString("%1-%2").arg(alias, type->name()));
    }

    ExtendedTypesList Scope::extendedTypes() const
    {
        return m_ExtendedType.values();
    }

} // namespace entity
