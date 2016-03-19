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

#include "association.h"

namespace relationship {

    /**
     * @brief The MultiplyAssociation class
     */
    class MultiplyAssociation : public Association
    {
    public:
        MultiplyAssociation();
        MultiplyAssociation(const common::ID &tailTypeId, const common::ID &headTypeId,
                            const db::WeakTypeSearchers &typeSearchers);

        friend bool operator ==(const MultiplyAssociation &lhs, const MultiplyAssociation &rhs);

        common::ID containerTypeId() const;
        void setContainerTypeId(const common::ID &containerTypeId);

        common::ID keyTypeId() const;
        void setKeyTypeId(const common::ID &keyTypeId);

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        bool isEqual(const MultiplyAssociation &rhs) const;

        add_meta(MultiplyAssociation)

    protected:
        void make() override;
        void clear() override;

        void makeGetter() override;
        void makeSetter() override;
        void makeField() override;
        void makeDeleter();
        void makeGroupGetter();
        void removeGetter() override;
        void removeSetter() override;
        void removeField() override;
        void removeDeleter();
        void removeGroupGetter();

        entity::SharedType m_ContainerClass;
        common::ID m_ContainerTypeId;
        common::ID m_KeyTypeId;
    };

} // namespace relationship
