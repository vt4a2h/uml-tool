#include "enum.h"
#include <algorithm>
#include <utility>

namespace entity {

    Enum::Enum()
        : Enum("noname", nullptr)
    {
    }

    Enum::Enum(const QString &name, Scope *scope)
        : Type(name, scope)
        , m_EnumType(nullptr)
        , m_StrongStatus(false)
    {
    }

    Type *Enum::enumType() const
    {
        return m_EnumType;
    }

    void Enum::setEnumType(Type *enumType)
    {
        m_EnumType = enumType;
    }

    bool Enum::isStrong() const
    {
        return m_StrongStatus;
    }

    void Enum::setStrongStatus(bool status)
    {
        m_StrongStatus = status;
    }

    Enum::Variable &Enum::addVariable(const QString &name)
    {
        return *m_Variables.insert(name, std::make_pair(name, m_Variables.size()));
    }

    Enum::Variable Enum::getVariable(const QString &name) const
    {
        return m_Variables.value(name);
    }

    void Enum::removeVariable(const QString &name)
    {
        m_Variables.remove(name);
    }

    bool Enum::containsVariable(const QString &name)
    {
        return m_Variables.contains(name);
    }

    Enum::VariablesList Enum::variables() const
    {
        return m_Variables.values();
    }

} // namespace entity
