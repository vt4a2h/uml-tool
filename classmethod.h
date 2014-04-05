#pragma once

#include <QString>
#include "types.h"

class QJsonObject;

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

        bool isConst() const;
        void setConstStatus(bool newStatus);

        RhsIdentificator rhsIdentificator() const;
        void setRhsIdentificator(RhsIdentificator identificator);

        QString returnTypeId() const;
        void setReturnTypeId(const QString &returnTypeId);

        LhsIdentificatorsList lhsIdentificators() const;
        void addLhsIdentificator(LhsIdentificator identificator);
        bool containsLhsIdentficator(LhsIdentificator identificator) const;
        bool hasLhsIdentificators() const;
        void removeLhsIdentificator(LhsIdentificator identificator);

        SharedField getParameter(const QString &name);
        SharedField addParameter(const QString &name, const QString &typeId);
        bool containsParameter(const QString &name);
        bool hasParameters() const;
        void removeParameter(const QString &name);

        FieldsList parameters() const;

        virtual QJsonObject toJson() const;
        virtual void fromJson(const QJsonObject &src, QStringList &errorList);

    private:
        QString m_Name;
        Section m_Section;
        bool    m_ConstStatus;
        QString m_ReturnTypeId;

        Fields m_Parameters;

        RhsIdentificator  m_RhsIdentificator;
        LhsIdentificators m_LhsIdentificators;
    };

} // namespace entity
