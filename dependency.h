#pragma once

#include "relation.h"

namespace relationship {

    class Dependency : public Relation
    {
    public:
        Dependency();
        Dependency(const QString &tailTypeId, const QString &headTypeId,
                   db::Database *globalDatabase, db::Database *projectDatabase);

        entity::SharedMethod method() const;
        void setMethod(const entity::SharedMethod &method);

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

    protected:
        void clear() override;
        void make() override;

    private:
        entity::SharedMethod m_Method;
    };

} // namespace relationship
