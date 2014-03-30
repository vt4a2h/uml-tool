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

        QString id() const;
        void setId(const QString &id);

    private:
        QString m_Name;
        QString m_Id;
        Scope  *m_Scope;
    };

} // namespace entity
