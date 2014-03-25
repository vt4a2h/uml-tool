#pragma once

#include <QString>

namespace entity {

    class Scope;

    class Type
    {
    public:
        Type();
        Type(const QString &name, Scope *scope);

        QString name() const;
        void setName(const QString &name);

        Scope *scope() const;
        void setScope(Scope *scope);

    private:
        QString m_Name;
        Scope  *m_Scope;
    };

} // namespace entity
