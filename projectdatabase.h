#pragma once

#include "types.h"
#include "database.h"

namespace db {

    class ProjectDatabase : public Database
    {
    public:
        ProjectDatabase(const QString &name = "", const QString &path = "");

        void clear() override;

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

    private:
        relationship::Relations m_Relations;
    };

} // namespace db

