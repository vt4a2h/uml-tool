/*****************************************************************************
**
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 01/11/2015.
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

#include <QAbstractItemModel>

#include "basictreeitem.h"

namespace models {

    class ProjectTreeModel : public QAbstractItemModel
    {
        Q_OBJECT

    public:
        ProjectTreeModel(QObject * parent = nullptr);
        ~ProjectTreeModel();

        QVariant data(const QModelIndex &index, int role) const;
        Qt::ItemFlags flags(const QModelIndex &index) const;

        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

        QModelIndex parent(const QModelIndex &child) const;

        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;

    private:
        void fillData();
        BasicTreeItem *m_Root;
    };

} // namespace models
