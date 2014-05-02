#pragma once

#include "association.h"

namespace relationship {

    class MultiplyAssociation : public Association
    {
    public:
        MultiplyAssociation();
        MultiplyAssociation(const QString &tailTypeId, const QString &headTypeId,
                            db::Database *globalDatabase, db::Database *projectDatabase);

        QString containerTypeId() const;
        void setContainerTypeId(const QString &containerTypeId);

        QString keyTypeId() const;
        void setKeyTypeId(const QString &keyTypeId);

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

    protected:
        void make() override;
        void clear() override;

        void makeGetter() override;
        void makeSetter() override;
        void makeField() override;
        void makeDeleter();
        void makeGroupGetter();
        void removeGetter() override;
        void removeSetter() override;
        void removeField() override;
        void removeDeleter();
        void removeGroupGetter();

        entity::SharedType m_ContainerClass;
        QString m_ContainerTypeId;
        QString m_KeyTypeId;
    };

} // namespace relationship
