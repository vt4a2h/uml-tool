#pragma once

#include <QString>
#include "types.h"

namespace entity {

    class ExtendedType;
    class Field;
    enum Section : int;
    enum Identificator : int;

    class ClassMethod
    {
    public:
        ClassMethod();
        ClassMethod(const QString & name);

        QString name() const;
        void setName(const QString &name);

        Section kind() const;
        void setKind(Section &kind);

        ExtendedType *returnType() const;
        void setReturnType(ExtendedType *returnType);

        bool isConst() const;
        void setConstStatus(bool newStatus);

        Identificator identificator() const;
        void setIdentificator(const Identificator &identificator);

        SharedField getParameter(const QString &name);
        SharedField addParameter(const QString &name, ExtendedType *type);
        bool containsParameter(const QString &name);
        void removeParameter(const QString &name);

        FieldsList parameters() const;

    private:
        QString m_Name;
        Section m_Kind;
        Identificator m_Identificator;
        bool          m_ConstStatus;
        ExtendedType *m_ReturnType;

        Fields m_Parameters;
    };

} // namespace entity
