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
        enum CustomRoles { SharedData = 32, DataType, ID };

        ProjectTreeModel(QObject * parent = nullptr);

        QVariant data(const QModelIndex &index, int role) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;

        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

        QModelIndex parent(const QModelIndex &child) const override;

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

        void addProject(const project::SharedProject &pr);

        void addScope(const entity::SharedScope &scope, const QString &projectId);
        void removeScope(const QString &scopeId, const QString &projectId);

        void addType(const entity::SharedType &type, const QString &scopeId, const QString &projectId);
        void removeType(const QString &projectID, const QString &scopeID, const QString &typeID);

    private:
        void addProjectItem(const project::SharedProject &pr);
        int indexOf(const BasicTreeItem *parent);
        BasicTreeItem *find(const QString &id);
        const BasicTreeItem *find(const QString &id) const;

        mutable QList<BasicTreeItem> m_Items; // TODO: store as pointers
    };

} // namespace models
