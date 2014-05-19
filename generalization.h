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
                       db::Database *globalDatabase, db::Database *projectDatabase);

        entity::Section section() const;
        void setSection(const entity::Section &section);

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

    protected:
        void clear() override;
        void make() override;

        entity::Section m_Section;
    };

} // namespace relationship
