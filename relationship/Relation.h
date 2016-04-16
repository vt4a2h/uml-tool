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

#include <db/db_types.hpp>

#include <common/meta.h>
#include <common/basicelement.h>

#include <entity/entity_types.hpp>

#include "relationship_types.hpp"

class QJsonObject;

namespace db {
    class Database;
}

namespace relationship {

    enum RelationType : int;

    // TODO: use copy-and-swap for copying. And in other classes too.

    /**
     * @brief The Relation class
     */
    class Relation : public common::BasicElement
    {
    public:
        Relation();
        Relation(Relation &&src) noexcept = default;
        Relation(const Relation &src);
        Relation(const common::ID &tailTypeId, const common::ID &headTypeId,
                 const db::WeakTypeSearchers &typeSearchers);

        Relation &operator =(const Relation &rhs);
        Relation &operator =(Relation &&rhs) noexcept = default;

        friend bool operator ==(const Relation &lhs, const Relation &rhs);

        void makeRelation();
        void removeRelation();

        RelationType relationType() const;
        void setRelationType(const RelationType &relationType);

        entity::SharedType headType() const;
        void setHeadType(const entity::SharedType &type);

        entity::SharedType tailType() const;
        void setTailType(const entity::SharedType &type);

        void setType(const SharedNode &node, const common::ID &typeId);

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        virtual bool isEqual(const Relation &rhs) const;

        db::WeakTypeSearchers typeSearchers() const;
        void setTypeSearchers(const db::WeakTypeSearchers &typeSearchers);

        static QString typeMark();

        add_meta(Relation)

    protected:
        virtual void make();
        virtual void clear();

        entity::SharedClass headClass() const;
        entity::SharedClass tailClass() const;

        virtual void copyFrom(const Relation &src);

        void check();
        entity::SharedType findType(const common::ID &typeId) const;

        SharedNode m_TailNode;
        SharedNode m_HeadNode;

        RelationType m_RelationType;

        db::WeakTypeSearchers m_TypeSearchers;
    };

} // namespace relationship
