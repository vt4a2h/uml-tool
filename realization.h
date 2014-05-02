#pragma once

#include "types.h"
#include "generalization.h"

namespace relationship {

    class Realization : public Generalization
    {
    public:
        Realization();
        Realization(const QString &tailTypeId, const QString &headTypeId,
                    db::Database *globalDatabase, db::Database *projectDatabase);

        void addMethods(const entity::MethodsList &methods);
        entity::MethodsList methods() const;

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

    protected:
        void clear() override;
        void make() override;

    private:
        entity::MethodsList m_Methods;
    };

} // namespace relationship
