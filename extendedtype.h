#pragma once

#include <utility>
#include <QString>
#include <QList>

#include "types.h"

namespace entity {

    class Scope;
    class Type;

    class ExtendedType
    {
    public:
        using Pl     = std::pair<QString, bool>;
        using PlList = QList<Pl>;

        ExtendedType();
        ExtendedType(Scope *scope, Type *type, const QString &alias = "");
        ~ExtendedType();

        bool isLink() const;
        void addPointerStatus(bool pointerToConst = false);
        void removePointerStatus();

        bool isPointer() const;
        void addLinkStatus();
        void removeLinkStatus();

        PlList pl() const;

        bool isConst() const;
        void setConstStatus(bool status);

        Scope *scope() const;
        void setScope(Scope *scope);

        Type *type() const;
        void setType(Type *type);

        QString alias() const;
        void setAlias(const QString &alias);

        void addTemplateParameter(ExtendedType *t);
        bool containsTemplateParameter(ExtendedType *t) const;
        void removeTemplateParameters(ExtendedType *t);
        ExtendedTypesRawList templateParameters() const;

        QString id() const;
        void setId(const QString &id);

    protected:
        bool    m_ConstStatus;
        Scope  *m_Scope;
        Type   *m_Type;
        QString m_Alias;
        QString m_Id;
        PlList  m_Pl;
        ExtendedTypesRawList m_TemplateParameters;
    };

} // namespace entity
