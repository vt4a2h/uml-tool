#pragma once

#include "association.h"

namespace relationship {

    class MultiplyAssociation : public Association
    {
    public:
        MultiplyAssociation();
        MultiplyAssociation(const QString &tailTypeId, const QString &headTypeId);
    };

} // namespace relationship
