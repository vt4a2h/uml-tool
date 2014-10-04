#pragma once

#include "type.h"
#include "types.h"
#include "enums.h"

namespace entity {

    class   Class : public Type
    {
    public:
        Class();
        Class(Class &&src);
        Class(const Class &src);
        Class(const QString &name, const QString &scopeId);

        Class &operator =(Class &&rhs);
        Class &operator =(Class rhs);

        friend bool operator ==(const Class &lhs, const Class &rhs);

        Parent addParent(const QString &typeId, Section section);
        ParentsList getParents(const QString &typeId);
        bool containsParent(const QString &typeId);
        void removeParent(const QString &typeId);
        bool anyParents() const;
        ParentsList parents() const;

        template <class T = ClassMethod> std::shared_ptr<T> makeMethod(const QString &name);
        void addMethod(SharedMethod method);
        MethodsList getMethod(const QString &name);
        bool containsMethod(const QString &name);
        void removeMethods(const QString &name);
        void removeMethod(const SharedMethod &method);
        MethodsList methods() const;
        bool anyMethods() const;

        bool containsMethods(Section section) const;
        MethodsList methods(Section section) const;

        SharedField addField(const QString &name, const QString &typeId,
                             const QString prefix = "", Section section = Public);
        SharedField getField(const QString &name) const;
        bool containsField(const QString &name) const;
        void removeField(const QString &name);
        FieldsList fields() const;
        bool anyFields() const;

        bool containsFields(Section section) const;
        FieldsList fields(Section section) const;

        Kind kind() const;
        void setKind(Kind kind);

        bool isFinal() const;
        void setFinalStatus(bool status);

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        bool isEqual(const Class &rhs) const;

    protected:
        void moveFrom(Class &src);
        void copyFrom(const Class &src);

    private:
        Kind m_Kind;
        bool m_FinalStatus;

        ParentsList m_Parents;
        MethodsList m_Methods;
        FieldsList  m_Fields;
    };

    template <class T>
    std::shared_ptr<T> Class::makeMethod(const QString &name)
    {
        typedef typename std::conditional<std::is_class<T>::value && std::is_base_of<ClassMethod, T>::value, T, ClassMethod>::type ResultType;
        auto value = std::make_shared<ResultType>(name);
        m_Methods << value;

        return value;
    }

} // namespace entity
