/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 24/05/2015.
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
#include "classcomponentsmodel.h"

namespace gui {

    /**
     * @brief ClassComponentsModel::ClassComponentsModel
     * @param parent
     */
    ClassComponentsModel::ClassComponentsModel(QObject *parent)
        : QAbstractTableModel(parent)
    {
    }

    /**
     * @brief ClassComponentsModel::rowCount
     * @param parent
     * @return
     */
    int ClassComponentsModel::rowCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent);
        return 0;
    }

    /**
     * @brief ClassComponentsModel::columnCount
     * @param parent
     * @return
     */
    int ClassComponentsModel::columnCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent);
        return 0;
    }

    /**
     * @brief ClassComponentsModel::data
     * @param index
     * @param role
     * @return
     */
    QVariant ClassComponentsModel::data(const QModelIndex &index, int role) const
    {
        Q_UNUSED(index); Q_UNUSED(role);
        return QVariant();
    }

} // namespace gui
