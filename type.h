#pragma once

#include <QString>

class QJsonObject;

namespace entity {

    class Scope;
    enum UserType : int;

    class Type
    {
    public:
        Type();
        Type(Type &&src);
        Type(const Type &src);
        Type(const QString &name, const QString &scopeId);
        virtual ~Type();

        Type &operator =(Type &&rhs);
        Type &operator =(Type rhs);

        QString name() const;
        void setName(const QString &name);

        QString id() const;
        void setId(const QString &id);

        QString scopeId() const;
        void setScopeId(const QString &scopeId);

        UserType type() const;

        virtual QJsonObject toJson() const;
        virtual void fromJson(const QJsonObject &src, QStringList &errorList);

    protected:
        virtual void moveFrom(Type &src);

        UserType m_KindOfType;

    private:
        QString m_Name;
        QString m_Id;
        QString m_ScopeId;
    };

} // namespace entity
