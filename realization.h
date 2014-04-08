#pragma once

#include "types.h"
#include "generalization.h"

namespace relationship {

    class Realization : public Generalization
    {
    public:
        Realization();
        Realization(const QString &tailTypeId, const QString &headTypeId);

        void addMethods(const entity::MethodsList &methods);
        entity::MethodsList methods() const;

        void make() override;
        void clear() override;

    private:
        entity::MethodsList m_Methods;
    };

} // namespace relationship
