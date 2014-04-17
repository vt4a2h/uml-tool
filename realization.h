#pragma once

#include "types.h"
#include "generalization.h"

namespace relationship {

    class Realization : public Generalization
    {
    public:
        Realization();
        Realization(const QString &tailTypeId, const QString &headTypeId,
                    const db::SharedDatabase &globalDatabase, const db::SharedDatabase &projectDatabase);

        void addMethods(const entity::MethodsList &methods);
        entity::MethodsList methods() const;

        void clear() override;

    protected:
        void make() override;

    private:
        entity::MethodsList m_Methods;
    };

} // namespace relationship
