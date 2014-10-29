#pragma once

#include "relation.h"

/**
 * @brief relationship
 */
namespace relationship {

    /**
     * @brief The Association class
     */
    class Association : public Relation
    {
    public:
        Association();
        Association(const QString &tailTypeId, const QString &headTypeId,
                    db::Database *globalDatabase, db::Database *projectDatabase);

        friend bool operator ==(const Association &lhs, const Association &rhs);

        QString getGetSetTypeId() const;
        void setGetSetTypeId(const QString &getSetTypeId);

        QString fieldtypeId() const;
        void setFieldtypeId(const QString &fieldtypeId);

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        bool isEqual(const Association &rhs) const;

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
        QString m_FieldTypeId;
    };

} // namespace relationship
