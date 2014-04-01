#include "enum.h"
#include "constants.cpp"
#include <algorithm>
#include <utility>

namespace entity {

    Enum::Enum()
        : Enum(DEFAULT_NAME, GLOBAL_SCOPE_ID)
    {
    }

    Enum::Enum(const QString &name, const QString &scopeId)
        : Type(name, scopeId)
        , m_EnumTypeId(STUB_ID)
        , m_StrongStatus(false)
    {
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

    QString Enum::enumTypeId() const
    {
        return m_EnumTypeId;
    }

    void Enum::setEnumTypeId(const QString &enumTypeId)
    {
        m_EnumTypeId = enumTypeId;
    }


} // namespace entity
