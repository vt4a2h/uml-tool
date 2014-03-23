#pragma once

#include "type.h"
#include <utility>
#include <QList>

namespace entity {

    class Enum : public Type
    {
    public:
        using Variable  = std::pair<QString, unsigned int>;
        using Variables = QList<Variable>;

        Enum();
        Enum(const QString &name, Scope *scope);

        Type *enumType() const;
        void setEnumType(Type *enumType);

        bool isStrong() const;
        void setStrongStatus(bool status);

        bool addVariable(const Variable &variable);
        void removeVariable(const QString &name);
        bool containsVariable(const QString &name);
        Variables variables() const;

    private:
        Type *m_EnumType;
        bool m_StrongStatus;
        Variables m_Variables;
    };

} // namespace entity
