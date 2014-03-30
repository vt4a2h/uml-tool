#include "type.h"
#include "scope.h"
#include "helpfunctions.h"

namespace entity {

    Type::Type()
        : Type("noname", nullptr)
    {
    }

    Type::Type(const QString &name, Scope *scope)
        : m_Name(name)
        , m_Id(utility::genId())
        , m_Scope(scope)
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

    Scope *Type::scope() const
    {
        return m_Scope;
    }

    void Type::setScope(Scope *scope)
    {
        m_Scope = scope;
    }

    QString Type::id() const
    {
        return m_Id;
    }
    
    void Type::setId(const QString &id)
    {
        m_Id = id;
    }   
    
} // namespace entity
