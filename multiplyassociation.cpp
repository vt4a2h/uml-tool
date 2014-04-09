#include "multiplyassociation.h"
#include "enums.h"
#include "constants.cpp"

namespace relationship {

    MultiplyAssociation::MultiplyAssociation()
        : MultiplyAssociation(STUB_ID, STUB_ID)
    {
    }

    MultiplyAssociation::MultiplyAssociation(const QString &tailTypeId, const QString &headTypeId)
        : Association(tailTypeId, headTypeId)
    {
        m_RelationType = MultiRelation;
    }

} // namespace relationship
