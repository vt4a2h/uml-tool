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

        friend bool operator ==(const Realization &lhs, const Realization &rhs);

        void addMethods(const entity::MethodsList &methods);
        entity::MethodsList methods() const;

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        bool isEqual(const Realization &rhs) const;

    protected:
        void clear() override;
        void make() override;

    private:
        entity::MethodsList m_Methods;
    };

} // namespace relationship
