#include "type.h"
#include "scope.h"
#include "helpfunctions.h"
#include "constants.cpp"

namespace entity {

    Type::Type()
        : Type(DEFAULT_NAME, GLOBAL_SCOPE_ID)
    {
    }

    Type::Type(const QString &name, const QString &scopeId)
        : m_Name(name)
        , m_Id(utility::genId())
        , m_ScopeId(scopeId)
    {
    }

    QString Type::name() const
    {
        return m_Name;
    }

    void Type::setName(const QString &name)
    {
        m_Name = name;
    }

    QString Type::id() const
    {
        return m_Id;
    }
    
    void Type::setId(const QString &id)
    {
        m_Id = id;
    }

    QString Type::scopeId() const
    {
        return m_ScopeId;
    }
    
    void Type::setScopeId(const QString &scopeId)
    {
        m_ScopeId = scopeId;
    }
        
} // namespace entity
