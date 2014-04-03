#pragma once

#include <QString>

class QJsonObject;

namespace entity {

    class Scope;

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

        virtual QJsonObject toJson() const;
        virtual void fromJson(const QJsonObject &src, QStringList &errorList);

    private:
        QString m_Name;
        QString m_Id;
        QString m_ScopeId;
    };

} // namespace entity
