#pragma once

#include "types.h"
#include "database.h"

namespace db {

    class ProjectDatabase : public Database
    {
    public:
        ProjectDatabase(ProjectDatabase &&src);
        ProjectDatabase(const ProjectDatabase &src);
        ProjectDatabase(const QString &name = "", const QString &path = "");

        ProjectDatabase &operator =(ProjectDatabase &&rhs);
        ProjectDatabase &operator =(ProjectDatabase rhs);

        relationship::SharedRelation getRelation(const QString &id) const;
        void addRelation(const relationship::SharedRelation &relation);
        bool containsRelation(const QString &id) const;
        void removeRelation(const QString &id);
        relationship::RelationsList relations() const;

        db::SharedDatabase globalDatabase() const;
        void setGlobalDatabase(const db::SharedDatabase &globalDatabase);

        void clear() override;

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

    protected:
        virtual void copyFrom(const ProjectDatabase &src);
        virtual void moveFrom(ProjectDatabase &src);

    private:
        relationship::Relations m_Relations;
        db::SharedDatabase m_GlobalDatabase;
    };

} // namespace db

