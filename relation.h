#pragma once

#include "types.h"
#include <QString>

class QJsonObject;

namespace db {
    class Database;
}

namespace relationship {

    enum RelationType : int;

    class Relation
    {
    public:
        Relation();
        Relation(const QString &tailTypeId, const QString &headTypeId,
                 const db::SharedDatabase &globalDatabase, const db::SharedDatabase &projectDatabase);
        virtual ~Relation();

        QString description() const;
        void setDescription(const QString &description);

        virtual void make();
        virtual void clear();

        RelationType relationType() const;
        void setRelationType(const RelationType &relationType);

        QString id() const;
        void setId(const QString &id);

        QJsonObject toJson() const;
        void fromJson(const QJsonObject &src, QStringList &errorList);

        db::SharedDatabase globalDatabase() const;
        void setGlobalDatabase(const db::SharedDatabase &globalDatabase);

        db::SharedDatabase projectDatabase() const;
        void setProjectDatabase(const db::SharedDatabase &projectDatabase);

    protected:
        entity::SharedType tryToFindType(const QString &typeId) const;

        SharedNode m_TailNode;
        SharedNode m_HeadNode;

        // FIXME: add casts to Class pointers in Association, Dependency, Realization etc.
        entity::SharedType m_HeadClass;
        entity::SharedType m_TailClass;

        QString m_Id;
        QString m_Description;
        RelationType m_RelationType;

        db::SharedDatabase m_GlobalDatabase;
        db::SharedDatabase m_ProjectDatabase;
    };

} // namespace relationship
