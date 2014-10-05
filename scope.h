#pragma once

#include <type_traits>
#include <QString>
#include "types.h"

namespace entity {

    enum UserType : int;

    class Scope
    {
    public:
        Scope(Scope &&src);
        Scope(const Scope &src);
        Scope(const QString &scopeName = "", const QString &scopeId = "");

        Scope &operator =(Scope rhs);
        Scope &operator =(Scope &&rhs);

        friend bool operator ==(const Scope &lhs, const Scope &rhs);

        QString name() const;
        void setName(const QString &name);

        SharedType getType(const QString &typeId) const;
        SharedType takeType(const QString &typeId);
        template <class T = Type> std::shared_ptr<T> addType(const QString &name = "");
        void addClonedType(const SharedType &type);
        template <class T = SharedType> T addExistsType(const T &type);
        bool containsType(const QString &typeId) const;
        void removeType(const QString &typeId);
        TypesList types() const;

        SharedScope getChildScope(const QString &typeId);
        SharedScope takeChildScope(const QString &typeId);
        SharedScope addChildScope(const QString &name = "");
        bool containsChildScope(const QString &typeId);
        bool hasChildScopes() const;
        void removeChildScope(const QString &typeId);
        ScopesList scopes() const;

        QString id() const;
        void setId(const QString &id);

        QString parentScopeId() const;
        void setParentScopeId(const QString &parentScopeId);

        QJsonObject toJson() const;
        void fromJson(const QJsonObject &src, QStringList &errorList);

        void writeToFile(const QString &fileName) const;
        bool readFromFile(const QString &fileName);

    private:
        void copyFrom(const Scope &src);
        void moveFrom(Scope &src);

        QString m_Name;
        QString m_Id;
        QString m_ParentScopeId;
        Scopes m_Scopes;
        Types  m_Types;
    };

    template <class T>
    std::shared_ptr<T> Scope::addType(const QString &name)
    {
        using ResultType = typename std::conditional<std::is_class<T>::value &&
                                                     std::is_base_of<Type, T>::value,
                                                     T, Type>::type;
        auto value = std::make_shared<ResultType>(name, m_Id);
        m_Types.insert(value->id(), value);
        return value;
    }

    template <class T>
    T Scope::addExistsType(const T &type)
    {
        type->setScopeId(m_Id);
        return *m_Types.insert(type->id(), type);
    }

} // namespace entity
