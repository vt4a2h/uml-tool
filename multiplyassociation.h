#pragma once

#include "association.h"

namespace relationship {

    class MultiplyAssociation : public Association
    {
    public:
        MultiplyAssociation();
        MultiplyAssociation(const QString &tailTypeId, const QString &headTypeId,
                            const db::SharedDatabase &globalDatabase, const db::SharedDatabase &projectDatabase);

        void clear() override;

        QString containerTypeId() const;
        void setContainerTypeId(const QString &containerTypeId);

        QString indexTypeId() const;
        void setIndexTypeId(const QString &indexTypeId);

    protected:
        void make() override;

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


        QString m_ContainerTypeId;
        QString m_IndexTypeId;
    };

} // namespace relationship
