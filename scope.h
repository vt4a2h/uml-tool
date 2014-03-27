#pragma once

#include <QString>
#include "types.h"

namespace entity {

    enum UserType : int;

    class Scope
    {
    public:
        Scope(const QString &scopeName = "");

        QString name() const;
        void setName(const QString &name);

        Scope *parentScope() const;
        void setParentScope(Scope *parentScope);

        SharedType getType(const QString &name) const;
        SharedType addType(const QString &name, UserType kindOfType);
        bool containsType(const QString &name) const;
        void removeType(const QString &name);
        TypesList types() const;

    private:
        void swap(Scope &other);

        QString m_Name;
        Scope  *m_ParentScope;
        Types m_Types;
    };

} // namespace entity
