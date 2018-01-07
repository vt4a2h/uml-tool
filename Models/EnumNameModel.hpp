/*****************************************************************************
**
** Copyright (C) 2018 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 07-01-2018.
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

#include <Entity/EntityTypes.hpp>

#include "ISectionDataModel.hpp"

namespace Models {

    class EnumNameModel : public ISectionalDataModel
    {
    public:
        EnumNameModel(const Entity::SharedEnum &_enum);

    public: // QAbstractItemModel interface
        int rowCount(const QModelIndex &parent) const override;
        int columnCount(const QModelIndex &parent) const override;
        QVariant data(const QModelIndex &index, int role) const override;

    private: // Methods
        QVariant processDisplayRole(const QModelIndex & index) const;

    private: // Data
        Entity::SharedEnum m_Enum;
    };

} // namespace Models
