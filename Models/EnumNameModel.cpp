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
#include "EnumNameModel.hpp"

#include <Entity/Enum.h>

namespace Models {

    EnumNameModel::EnumNameModel(const Entity::SharedEnum &_enum)
        : m_Enum(_enum)
    {
    }

    int EnumNameModel::rowCount(const QModelIndex &/*parent*/) const
    {
        return 1;
    }

    int EnumNameModel::columnCount(const QModelIndex &/*parent*/) const
    {
        return 1;
    }

    QVariant EnumNameModel::data(const QModelIndex &index, int role) const
    {
        if (!m_Enum || !index.isValid())
            return QVariant();

        switch (role) {
            case Qt::DisplayRole:
                return processDisplayRole(index);

            default:
                return QVariant();
        }
    }

    QVariant EnumNameModel::processDisplayRole(const QModelIndex &index) const
    {
        Q_ASSERT(index.isValid() && m_Enum);

        if (index.row() != 0 || index.column() != 0)
            return QVariant();

        // TODO: use converter class to generate enum name
        return m_Enum->name();
    }

} // namespace Models
