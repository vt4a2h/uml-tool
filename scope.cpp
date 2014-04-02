#include "scope.h"
#include "type.h"
#include "class.h"
#include "templateclass.h"
#include "union.h"
#include "enum.h"
#include "enums.h"
#include "extendedtype.h"
#include "helpfunctions.h"
#include "constants.cpp"
#include <utility>

namespace entity {

    Scope::Scope(const QString &scopeName, const QString &scopeId)
        : m_Name(!scopeName.isEmpty() ? scopeName : DEFAULT_NAME)
        , m_Id(utility::genId())
        , m_ParentScopeId(!scopeId.isEmpty() ? scopeId : GLOBAL_SCOPE_ID)
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

    SharedType Scope::getType(const QString &typeId) const
    {
        return (m_Types.contains(typeId) ? m_Types[typeId] : nullptr);
    }

    SharedType Scope::takeType(const QString &typeId)
    {
        SharedType result(nullptr);

        if (m_Types.contains(typeId)) {
            result = m_Types[typeId];
            m_Types.remove(typeId);
        }

        return result;
    }

    bool Scope::containsType(const QString &typeId) const
    {
        return m_Types.contains(typeId);
    }

    void Scope::removeType(const QString &typeId)
    {
        m_Types.remove(typeId);
    }

    TypesList Scope::types() const
    {
        return m_Types.values();
    }

    SharedExtendedType Scope::getExtendedType(const QString &typeId) const
    {
        return (m_ExtendedType.contains(typeId) ? m_ExtendedType[typeId] : nullptr);
    }

    SharedExtendedType Scope::takeExtendedType(const QString &typeId)
    {
        SharedExtendedType result(nullptr);

        if (m_ExtendedType.contains(typeId)) {
            result = m_ExtendedType[typeId];
            m_ExtendedType.remove(typeId);
        }

        return result;
    }

    SharedExtendedType Scope::addExtendedType()
    {
        auto type = std::make_shared<ExtendedType>();
        type->setScopeId(m_Id);

        return *m_ExtendedType.insert(type->id(), type);
    }

    bool Scope::containsExtendedType(const QString &typeId) const
    {
        return m_ExtendedType.contains(typeId);
    }

    void Scope::removeExtendedType(const QString &typeId)
    {
        m_ExtendedType.remove(typeId);
    }

    ExtendedTypesList Scope::extendedTypes() const
    {
        return m_ExtendedType.values();
    }

    SharedScope Scope::getChildScope(const QString &typeId)
    {
        return (m_Scopes.contains(typeId) ? m_Scopes[typeId] : nullptr);
    }

    SharedScope Scope::takeChildScope(const QString &typeId)
    {
        SharedScope result(nullptr);

        if (m_Scopes.contains(typeId)) {
            result = m_Scopes[typeId];
            m_Scopes.remove(typeId);
        }

        return result;
    }

    SharedScope Scope::addChildScope(const QString &typeId)
    {
        return *m_Scopes.insert(typeId, std::make_shared<Scope>(typeId, m_Id));
    }

    bool Scope::containsChildScope(const QString &typeId)
    {
        return m_Scopes.contains(typeId);
    }

    void Scope::removeChildScope(const QString &typeId)
    {
        m_Scopes.remove(typeId);
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

    QString Scope::parentScopeId() const
    {
        return m_ParentScopeId;
    }
    
    void Scope::setParentScopeId(const QString &parentScopeId)
    {
        m_ParentScopeId = parentScopeId;
    }
    
} // namespace entity
