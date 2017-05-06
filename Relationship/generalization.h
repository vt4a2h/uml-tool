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

#include "Relation.h"

namespace Entity {
    enum Section : int;
}

namespace Relationship {

    /**
     * @brief The Generalization class
     */
    class Generalization : public Relation
    {
    public:
        Generalization();
        Generalization(const Common::ID &tailTypeId, const Common::ID &headTypeId,
                       const DB::WeakTypeSearchers &typeSearchers);

        friend bool operator ==(const Generalization &lhs, const Generalization &rhs);

        Entity::Section section() const;
        void setSection(const Entity::Section &section);

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        bool isEqual(const Generalization &rhs) const;

        add_meta(Generalization)

    protected:
        void clear() override;
        void make() override;

        Entity::Section m_Section;
    };

} // namespace relationship
