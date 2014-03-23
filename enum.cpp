#include "enum.h"
#include <algorithm>

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

    bool Enum::addVariable(const Enum::Variable &variable)
    {
        bool operationStatus(false);

        if (!containsVariable(variable.first)) {
            m_Variables.append(variable);
            operationStatus = true;
        }

        return operationStatus;
    }

    void Enum::removeVariable(const QString &name)
    {
        std::remove_if(m_Variables.begin(), m_Variables.end(),
                       [&](const Variable &v){ return v.first == name; });
    }

    bool Enum::containsVariable(const QString &name)
    {
        return std::find_if(m_Variables.begin(), m_Variables.end(),
                            [&](const Variable &v){ return v.first == name; }) != m_Variables.end();
    }

    Enum::Variables Enum::variables() const
    {
        return m_Variables;
    }


} // namespace entity
