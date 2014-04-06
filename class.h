#pragma once

#include "type.h"
#include "types.h"

namespace entity {

    class ExtendedType;
    enum Kind    : int;
    enum Section : int;

    class Class : public Type
    {
    public:
        Class();
        Class(const QString &name, const QString &scopeId);

        Parent addParent(const QString &typeId, Section section);
        ParentsList getParents(const QString &typeId);
        bool containsParent(const QString &typeId);
        void removeParent(const QString &typeId);
        ParentsList parents() const;

        template <class T = ClassMethod> std::shared_ptr<T> addMethod(const QString &name);
        MethodsList  getMethod(const QString &name);
        bool containsMethod(const QString &name);
        void removeMethods(const QString &name);
        void removeMethod(SharedMethod method);
        MethodsList methods() const;

        SharedField addField(const QString &name, const QString &typeId);
        SharedField getField(const QString &name);
        bool containsField(const QString &name);
        void removeField(const QString &name);
        FieldsList fields() const;

        Kind kind() const;
        void setKind(Kind kind);

        bool isFinal() const;
        void setFinalStatus(bool status);

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

    private:
        Kind m_Kind;
        bool m_FinalStatus;

        Parents m_Parents;
        Methods m_Methods;
        Fields  m_Fields;
    };

    template <class T>
    std::shared_ptr<T> Class::addMethod(const QString &name)
    {
        typedef typename std::conditional<std::is_class<T>::value && std::is_base_of<ClassMethod, T>::value, T, ClassMethod>::type ResultType;
        auto value = std::make_shared<ResultType>(name);
        m_Methods.insertMulti(name, value);

        return value;
    }

} // namespace entity
