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
        Relation(Relation &&src);
        Relation(const Relation &src);
        Relation(const QString &tailTypeId, const QString &headTypeId,
                 db::Database *globalDatabase, db::Database *projectDatabase);
        virtual ~Relation();

        Relation &operator =(Relation rhs);
        Relation &operator =(Relation &&rhs);

        QString description() const;
        void setDescription(const QString &description);

        void makeRelation();
        void removeRelation();

        RelationType relationType() const;
        void setRelationType(const RelationType &relationType);

        QString id() const;
        void setId(const QString &id);

        db::Database *globalDatabase() const;
        void setGlobalDatabase(db::Database *globalDatabase);

        db::Database *projectDatabase() const;
        void setProjectDatabase(db::Database *projectDatabase);

        virtual QJsonObject toJson() const;
        virtual void fromJson(const QJsonObject &src, QStringList &errorList);

    protected:
        virtual void make();
        virtual void clear();

        virtual void moveFrom(Relation &src);
        virtual void copyFrom(const Relation &src);

        void check();
        void addHeadClass(const QString &id);
        void addTailClass(const QString &id);
        entity::SharedType tryToFindType(const QString &typeId) const;

        SharedNode m_TailNode;
        SharedNode m_HeadNode;

        entity::SharedType m_HeadClass;
        entity::SharedClass m_TailClass;

        QString m_Id;
        QString m_Description;
        RelationType m_RelationType;

        db::Database *m_GlobalDatabase;
        db::Database *m_ProjectDatabase;
    };

} // namespace relationship
