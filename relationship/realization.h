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

#include <QVector>

#include "types.h"
#include "generalization.h"

namespace relationship {

    /**
     * @brief The Realization class
     */
    class Realization : public Generalization
    {
    public:
        Realization();
        Realization(const common::ID &tailTypeId, const common::ID &headTypeId,
                    const db::WeakTypeSearchers &typeSearchers);

        friend bool operator ==(const Realization &lhs, const Realization &rhs);

        void addMethods(const entity::MethodsList &methods);
        entity::MethodsList methods() const;

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        bool isEqual(const Realization &rhs) const;

        add_meta(Realization)

    protected:
        void clear() override;
        void make() override;

    private:
        entity::MethodsList m_Methods;
    };

} // namespace relationship
