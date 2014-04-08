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
        Generalization(const QString &tailTypeId, const QString &headTypeId);

        entity::Section section() const;
        void setSection(const entity::Section &section);

        void make() override;
        void clear() override;

    protected:
        entity::Section m_Section;
    };

} // namespace relationship
