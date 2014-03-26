#pragma once

#include "type.h"
#include "types.h"

namespace entity {

    enum Kind    : int;
    enum Section : int;

    class Class : public Type
    {
    public:
        Class();
        Class(const QString &name, Scope *scope);

        ClassParent addParent(SharedClass cl, Section section);
        ClassParent getParent(const QString &name, Scope *scope = nullptr);
        bool containsParent(const QString &name, Scope *scope = nullptr);
        void removeParent(const ClassParent &parent);
        void removeParent(const QString &name, Scope *scope = nullptr);
        ClassParents parents() const;

        Kind kind() const;
        void setKind(Kind kind);

        bool isFinal() const;
        void setFinalStatus(bool status);

    private:
        Kind m_Kind;
        bool m_FinalStatus;

        ClassParents m_Parents;
    };

} // namespace entity
