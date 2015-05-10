/*****************************************************************************
**
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 29/10/2014.
**
** This file is part of Q-UML (UML tool for Qt).
**
** Q-UML is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Q-UML is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.

** You should have received a copy of the GNU Lesser General Public License
** along with Q-UML.  If not, see <http://www.gnu.org/licenses/>.
**
*****************************************************************************/

#pragma once

#include <QString>

#include <entity/entity_types.hpp>

#include "relationship_types.hpp"

class QJsonObject;

namespace db {
    class Database;
}

namespace relationship {

    enum RelationType : int;

    /**
     * @brief The Relation class
     */
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

        friend bool operator ==(const Relation &lhs, const Relation &rhs);

        QString name() const;
        void setName(const QString &name);

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

        virtual bool isEqual(const Relation &rhs) const;

        void writeToFile(const QString &fileName) const;
        bool readFromFile(const QString &fileName);

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
        QString m_Name;
        RelationType m_RelationType;

        db::Database *m_GlobalDatabase;
        db::Database *m_ProjectDatabase;
    };

} // namespace relationship
