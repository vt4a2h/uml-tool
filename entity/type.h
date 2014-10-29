#pragma once

#include <QString>

class QJsonObject;

namespace entity {

    class Scope;
    enum UserType : int;

    /**
     * @brief The Type class
     */
    class Type
    {
    public:
        Type();
        Type(Type &&src);
        Type(const Type &src);
        Type(const QString &name, const QString &scopeId, const QString &typeId = "");
        virtual ~Type();

        Type &operator =(Type rhs);
        Type &operator =(Type &&rhs);
        friend bool operator ==(const Type &lhs, const Type &rhs);

        QString name() const;
        void setName(const QString &name);

        QString id() const;
        void setId(const QString &id);

        QString scopeId() const;
        void setScopeId(const QString &scopeId);

        UserType type() const;

        virtual QJsonObject toJson() const;
        virtual void fromJson(const QJsonObject &src, QStringList &errorList);

        void writeToFile(const QString &fileName) const;
        bool readFromFile(const QString &fileName);

        virtual Type *clone() const;

        virtual bool isEqual(const Type &rhs) const;

    protected:
        virtual void moveFrom(Type &src);
        virtual void copyFrom(const Type &src);

        UserType m_KindOfType;

    private:
        QString m_Name;
        QString m_Id;
        QString m_ScopeId;
    };

} // namespace entity
