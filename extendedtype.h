#pragma once

#include "type.h"
#include "types.h"

#include <QString>
#include <QList>

namespace entity {

    class ExtendedType : public Type
    {
    public:
        using Pl     = std::pair<QString, bool>; // (pointer or link, const for pointer)
        using PlList = QList<Pl>;

        ExtendedType();
        ExtendedType(const QString &name, const QString &scopeId);

        bool isLink() const;
        void addPointerStatus(bool pointerToConst = false);
        void removePointerStatus();

        bool isPointer() const;
        void addLinkStatus();
        void removeLinkStatus();

        PlList pl() const;

        bool isConst() const;
        void setConstStatus(bool status);

        void addTemplateParameter(const QString &typeId);
        bool containsTemplateParameter(const QString &typeId) const;
        void removeTemplateParameters(const QString &typeId);
        ExtendedTypesIdList templateParameters() const;

        QString typeId() const;
        void setTypeId(const QString &typeId);

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

    protected:
        bool    m_ConstStatus;
        QString m_TypeId;
        PlList  m_PointersAndLinks;

        ExtendedTypesIdList m_TemplateParameters;
    };

} // namespace entity
