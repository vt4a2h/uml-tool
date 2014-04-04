#pragma once

#include <type_traits>
#include <QString>
#include "types.h"

namespace entity {

    class Scope
    {
    public:
        Scope(const QString &scopeName = "", const QString &scopeId = "");

        QString name() const;
        void setName(const QString &name);

        SharedType getType(const QString &typeId) const;
        SharedType takeType(const QString &typeId);
        template <class T = Type> std::shared_ptr<T> addType(const QString &name = "");
        bool containsType(const QString &typeId) const;
        void removeType(const QString &typeId);
        TypesList types() const;

        SharedExtendedType getExtendedType(const QString &typeId) const;
        SharedExtendedType takeExtendedType(const QString &typeId);
        SharedExtendedType addExtendedType();
        bool containsExtendedType(const QString &typeId) const;
        void removeExtendedType(const QString &typeId);
        ExtendedTypesList extendedTypes() const;

        SharedScope getChildScope(const QString &typeId);
        SharedScope takeChildScope(const QString &typeId);
        SharedScope addChildScope(const QString &typeId);
        bool containsChildScope(const QString &typeId);
        void removeChildScope(const QString &typeId);
        ScopesList scopes() const;

        QString id() const;
        void setId(const QString &id);

        QString parentScopeId() const;
        void setParentScopeId(const QString &parentScopeId);

    private:
        void swap(Scope &other);

        QString m_Name;
        QString m_Id;
        QString m_ParentScopeId;
        Scopes m_Scopes;
        Types  m_Types;
        ExtendedTypes m_ExtendedType;
    };

    template <class T>
    std::shared_ptr<T> Scope::addType(const QString &name)
    {
        typedef typename std::conditional<std::is_class<T>::value && std::is_base_of<Type, T>::value, T, Type>::type ResultType;
        auto value = std::make_shared<ResultType>(name, m_Id);
        m_Types.insert(value->id(), value);
        return value;
    }

} // namespace entity
