#pragma once

#include "relation.h"

namespace relationship {

    class Dependency : public Relation
    {
    public:
        Dependency();
        Dependency(const QString &tailTypeId, const QString &headTypeId,
                   const db::SharedDatabase &globalDatabase, const db::SharedDatabase &projectDatabase);

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
