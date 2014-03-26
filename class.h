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
        Class(const QString &name, Scope *scope);

        Parent addParent(SharedClass cl, Section section);
        ParentsList getParents(const QString &name);
        bool containsParent(const QString &name);
        void removeParents(const QString &name);
        void removeParent(const Parent &parent);
        ParentsList parents() const;

        SharedMethod addMethod(const QString &name);
        MethodsList  getMethod(const QString &name);
        bool containsMethod(const QString &name);
        void removeMethods(const QString &name);
        void removeMethod(SharedMethod method);
        MethodsList methods() const;

        SharedField addField(const QString &name, ExtendedType *type);
        SharedField getField(const QString &name);
        bool containsField(const QString &name);
        void removeField(const QString &name);
        FieldsList fields() const;

        Kind kind() const;
        void setKind(Kind kind);

        bool isFinal() const;
        void setFinalStatus(bool status);

    private:
        Kind m_Kind;
        bool m_FinalStatus;

        Parents m_Parents;
        Methods m_Methods;
        Fields  m_Fields;
    };

} // namespace entity
