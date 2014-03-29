#pragma once

#include <QString>
#include "types.h"

namespace entity {

    enum UserType : int;

    class Scope
    {
    public:
        Scope(const QString &scopeName = "", Scope *parent = nullptr);

        QString name() const;
        void setName(const QString &name);

        Scope *parentScope() const;
        void setParentScope(Scope *parentScope);

        SharedType getType(const QString &name) const;
        SharedType takeType(const QString &name);
        SharedType addType(const QString &name, UserType kindOfType);
        void       addType(SharedType type, bool &operationStatus);
        bool containsType(const QString &name) const;
        void removeType(const QString &name);
        TypesList types() const;

        SharedExtendedType getExtendedType(const QString &alias, SharedType type) const;
        SharedExtendedType takeExtendedType(const QString &alias);
        SharedExtendedType addExtendedType(const QString &alias, SharedType type);
        void addExtendedType(SharedExtendedType extendedType, bool &operationStatus);
        bool containsExtendedType(const QString &alias, SharedType type) const;
        void removeExtendedType(const QString &alias, SharedType type);
        ExtendedTypesList extendedTypes() const;

        SharedScope getChildScope(const QString &name);
        SharedScope takeChildScope(const QString &name);
        SharedScope addChildScope(const QString &name);
        void        addChildScope(SharedScope scope, bool &operationStatus);
        bool containsChildScope(const QString &name);
        void removeChildScope(const QString &name);
        ScopesList scopes() const;

    private:
        void swap(Scope &other);

        QString m_Name;
        Scope  *m_ParentScope;
        Scopes m_Scopes;
        Types  m_Types;
        ExtendedTypes m_ExtendedType;
    };

} // namespace entity
