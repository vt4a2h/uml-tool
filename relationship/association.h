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

#include <common/id.h>

#include "relation.h"

/**
 * @brief relationship
 */
namespace relationship {

    /**
     * @brief The Association class
     */
    class Association : public Relation
    {
    public:
        Association();
        Association(const common::ID &tailTypeId, const common::ID &headTypeId,
                    db::Database *globalDatabase, db::Database *projectDatabase);

        friend bool operator ==(const Association &lhs, const Association &rhs);

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        bool isEqual(const Association &rhs) const;

        common::ID GetSetTypeId() const;
        void setGetSetTypeId(const common::ID &GetSetTypeId);

        common::ID FieldTypeId() const;
        void setFieldTypeId(const common::ID &FieldTypeId);

        add_meta(Association)

    protected:
        void make() override;
        void clear() override;

        virtual void makeField();
        virtual void removeField();
        virtual void makeGetter();
        virtual void makeSetter();
        virtual void removeGetter();
        virtual void removeSetter();

        common::ID m_GetSetTypeId;
        common::ID m_FieldTypeId;
    };

} // namespace relationship
