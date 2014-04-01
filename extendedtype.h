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
        ExtendedType(const QString &scopeId, const QString &typeId, const QString &alias = "");
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

        QString alias() const;
        void setAlias(const QString &alias);

        void addTemplateParameter(const QString &typeId);
        bool containsTemplateParameter(const QString &typeId) const;
        void removeTemplateParameters(const QString &typeId);
        ExtendedTypesRawList templateParameters() const;

        QString id() const;
        void setId(const QString &id);

        QString scopeId() const;
        void setScopeId(const QString &scopeId);

        QString typeId() const;
        void setTypeId(const QString &typeId);

    protected:
        bool    m_ConstStatus;
        QString m_ScopeId;
        QString m_TypeId;
        QString m_Alias;
        QString m_Id;
        PlList  m_Pl;

        ExtendedTypesRawList m_TemplateParameters;
    };

} // namespace entity
