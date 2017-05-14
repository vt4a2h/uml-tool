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

namespace Relationship {

    /**
     * @brief The Dependency class
     */
    class Dependency : public Relation
    {
    public:
        Dependency();
        Dependency(const Common::ID &tailTypeId, const Common::ID &headTypeId,
                   const DB::WeakTypeSearchers &typeSearchers);

        friend bool operator ==(const Dependency &lhs, const Dependency &rhs);

        Entity::SharedMethod method() const;
        void setMethod(const Entity::SharedMethod &method);

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        bool isEqual(const Dependency &rhs) const;

        add_meta(Dependency)

    protected:
        void clear() override;
        void make() override;

    private:
        Entity::SharedMethod m_Method;
    };

} // namespace relationship
