#pragma once

#include "relation.h"

namespace relationship {

    class Association : public Relation
    {
    public:
        Association();
        Association(const QString &tailTypeId, const QString &headTypeId,
                    const db::SharedDatabase &globalDatabase, const db::SharedDatabase &projectDatabase);

        QString getGetSetTypeId() const;
        void setGetSetTypeId(const QString &getSetTypeId);

        QString fieldtypeId() const;
        void setFieldtypeId(const QString &fieldtypeId);

    protected:
        void make() override;
        void clear() override;

        virtual void makeField();
        virtual void removeField();
        virtual void makeGetter();
        virtual void makeSetter();
        virtual void removeGetter();
        virtual void removeSetter();

        QString m_GetSetTypeId;
        QString m_FieldtypeId;
    };

} // namespace relationship
