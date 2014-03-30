#include "scope.h"
#include "type.h"
#include "class.h"
#include "templateclass.h"
#include "union.h"
#include "enum.h"
#include "enums.h"
#include "extendedtype.h"
#include "helpfunctions.h"
#include <utility>

namespace entity {

    Scope::Scope(const QString &scopeName, Scope *parent)
        : m_Name(scopeName)
        , m_Id(utility::genId())
        , m_ParentScope(parent)
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

    SharedType Scope::takeType(const QString &name)
    {
        SharedType result(nullptr);

        if (m_Types.contains(name)) {
            result = m_Types[name];
            m_Types.remove(name);
        }

        return result;
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

    void Scope::addType(SharedType type, bool &operationStatus)
    {
        operationStatus = false;
        if (!m_Types.contains(type->name())) {
            type->setScope(this);
            m_Types.insert(type->name(), type);
            operationStatus = true;
        }
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

    SharedExtendedType Scope::takeExtendedType(const QString &alias)
    {
        SharedExtendedType result(nullptr);

        if (m_ExtendedType.contains(alias)) {
            result = m_ExtendedType[alias];
            m_ExtendedType.remove(alias);
        }

        return result;
    }

    SharedExtendedType Scope::addExtendedType(const QString &alias, SharedType type)
    {
        return *m_ExtendedType.insert(QString("%1-%2").arg(alias, type->name()),
                                      std::make_shared<ExtendedType>(this, type.get(), alias));
    }

    void Scope::addExtendedType(SharedExtendedType extendedType, bool &operationStatus)
    {
        operationStatus = false;
        if (!m_ExtendedType.contains(extendedType->alias())) {
            extendedType->setScope(this);
            m_ExtendedType.insert(extendedType->alias(), extendedType);
            operationStatus = true;
        }
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

    SharedScope Scope::getChildScope(const QString &name)
    {
        return (m_Scopes.contains(name) ? m_Scopes[name] : nullptr);
    }

    SharedScope Scope::takeChildScope(const QString &name)
    {
        SharedScope result(nullptr);

        if (m_Scopes.contains(name)) {
            result = m_Scopes[name];
            m_Scopes.remove(name);
        }

        return result;
    }

    SharedScope Scope::addChildScope(const QString &name)
    {
        return *m_Scopes.insert(name, std::make_shared<Scope>(name, this));
    }

    void Scope::addChildScope(SharedScope scope, bool &operationStatus)
    {
        operationStatus = false;
        if (!m_Scopes.contains(scope->name())) {
            scope->setParentScope(this);
            m_Scopes.insert(scope->name(), scope);
            operationStatus = true;
        }
    }

    bool Scope::containsChildScope(const QString &name)
    {
        return m_Scopes.contains(name);
    }

    void Scope::removeChildScope(const QString &name)
    {
        m_Scopes.remove(name);
    }

    ScopesList Scope::scopes() const
    {
        return m_Scopes.values();
    }

    QString Scope::id() const
    {
        return m_Id;
    }
    
    void Scope::setId(const QString &id)
    {
        m_Id = id;
    }
    
    
} // namespace entity
