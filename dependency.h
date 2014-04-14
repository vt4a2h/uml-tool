#pragma once

#include "relation.h"

namespace relationship {

    class Dependency : public Relation
    {
    public:
        Dependency();
        Dependency(const QString &tailTypeId, const QString &headTypeId,
                   const db::SharedDatabase &globalDatabase, const db::SharedDatabase &projectDatabase);

        void make() override;
        void clear() override;

        entity::SharedMethod method() const;
        void setMethod(const entity::SharedMethod &method);

    private:
        entity::SharedMethod m_Method;
    };

} // namespace relationship
