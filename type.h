#pragma once

#include <QString>

namespace entity {

    class Scope;

    class Type
    {
    public:
        Type();
        Type(const QString &name, const QString &scopeId);

        QString name() const;
        void setName(const QString &name);

        QString id() const;
        void setId(const QString &id);

        QString scopeId() const;
        void setScopeId(const QString &scopeId);

    private:
        QString m_Name;
        QString m_Id;
        QString m_ScopeId;
    };

} // namespace entity
