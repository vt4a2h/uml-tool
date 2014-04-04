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
        Type(const QString &name, const QString &scopeId);
        virtual ~Type();

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
        UserType m_KindOfType;

    private:
        QString m_Name;
        QString m_Id;
        QString m_ScopeId;
    };

} // namespace entity
