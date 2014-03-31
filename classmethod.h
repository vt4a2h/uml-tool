#pragma once

#include <QString>
#include "types.h"

namespace entity {

    class ExtendedType;
    class Field;
    enum Section          : int;
    enum RhsIdentificator : int;
    enum LhsIdentificator : int;

    class ClassMethod
    {
    public:
        ClassMethod();
        ClassMethod(const QString & name);

        QString name() const;
        void setName(const QString &name);

        Section section() const;
        void setSection(Section &section);

        ExtendedType *returnType() const;
        void setReturnType(ExtendedType *returnType);

        bool isConst() const;
        void setConstStatus(bool newStatus);

        RhsIdentificator rhsIdentificator() const;
        void setRhsIdentificator(RhsIdentificator identificator);

        LhsIdentificatorsList lhsIdentificators() const;
        void addLhsIdentificator(LhsIdentificator identificator);
        bool containsLhsIdentficator(LhsIdentificator identificator) const;
        bool hasLhsIdentificators() const;
        void removeLhsIdentificator(LhsIdentificator identificator);

        SharedField getParameter(const QString &name);
        SharedField addParameter(const QString &name, ExtendedType *type);
        bool containsParameter(const QString &name);
        bool hasParameters() const;
        void removeParameter(const QString &name);

        FieldsList parameters() const;

    private:
        QString m_Name;
        Section m_Section;
        bool          m_ConstStatus;
        ExtendedType *m_ReturnType;

        Fields m_Parameters;

        RhsIdentificator  m_RhsIdentificator;
        LhsIdentificators m_LhsIdentificators;
    };

} // namespace entity
