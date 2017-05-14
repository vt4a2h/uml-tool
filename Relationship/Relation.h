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

#include <DB/db_types.hpp>

#include <Common/meta.h>
#include <Common/BasicElement.h>

#include <Entity/entity_types.hpp>

#include "relationship_types.hpp"

class QJsonObject;

namespace DB {
    class Database;
}

namespace Relationship {

    enum RelationType : int;

    // TODO: use copy-and-swap for copying. And in other classes too.

    /**
     * @brief The Relation class
     */
    class Relation : public Common::BasicElement
    {
        Q_OBJECT

    public:
        Relation();
        Relation(Relation &&src) noexcept = default;
        Relation(const Relation &src);
        Relation(const Common::ID &tailTypeId, const Common::ID &headTypeId,
                 const DB::WeakTypeSearchers &typeSearchers);

        Relation &operator =(const Relation &rhs);
        Relation &operator =(Relation &&rhs) noexcept = default;

        friend bool operator ==(const Relation &lhs, const Relation &rhs);

        void makeRelation();
        void removeRelation();

        RelationType relationType() const;
        void setRelationType(const RelationType &relationType);

        Entity::SharedType headType() const;
        void setHeadType(const Entity::SharedType &type);

        Entity::SharedType tailType() const;
        void setTailType(const Entity::SharedType &type);

        void setType(const SharedNode &node, const Common::ID &typeId);

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        virtual bool isEqual(const Relation &rhs) const;

        DB::WeakTypeSearchers typeSearchers() const;
        void setTypeSearchers(const DB::WeakTypeSearchers &typeSearchers);

        static QString typeMarker();

        add_meta(Relation)

    protected:
        virtual void make();
        virtual void clear();

        Entity::SharedClass headClass() const;
        Entity::SharedClass tailClass() const;

        virtual void copyFrom(const Relation &src);

        void check();
        Entity::SharedType findType(const Common::ID &typeId) const;

        SharedNode m_TailNode;
        SharedNode m_HeadNode;

        RelationType m_RelationType;

        DB::WeakTypeSearchers m_TypeSearchers;
    };

} // namespace relationship
