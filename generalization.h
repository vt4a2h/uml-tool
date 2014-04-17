#pragma once

#include "relation.h"

namespace entity {
    enum Section : int;
}

namespace relationship {

    class Generalization : public Relation
    {
    public:
        Generalization();
        Generalization(const QString &tailTypeId, const QString &headTypeId,
                       const db::SharedDatabase &globalDatabase, const db::SharedDatabase &projectDatabase);

        entity::Section section() const;
        void setSection(const entity::Section &section);

        void clear() override;

    protected:
        void make() override;
        entity::Section m_Section;
    };

} // namespace relationship
