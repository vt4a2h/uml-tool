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

#include "types.h"

namespace models {

    class ProjectTreeModel : public QAbstractItemModel
    {
        Q_OBJECT

    public:
        ProjectTreeModel(project::Projects &projects, QObject * parent = nullptr);

        QVariant data(const QModelIndex &index, int role) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;

        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

        QModelIndex parent(const QModelIndex &child) const override;

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        void fillData(); // to test period
    private:

        project::Projects &m_Projects;
        mutable QList<BasicTreeItem> m_Items;
    };

} // namespace models
