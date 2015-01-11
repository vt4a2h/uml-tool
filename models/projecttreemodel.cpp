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
#include "projecttreemodel.h"

namespace models {

    /**
     * @brief ProjectTreeModel::ProjectTreeModel
     * @param parent
     */
    ProjectTreeModel::ProjectTreeModel(QObject *parent)
        : QAbstractItemModel(parent)
        , m_Root(new BasicTreeItem(tr("Projects")))
    {
       fillData();
    }

    /**
     * @brief ProjectTreeModel::~ProjectTreeModel
     */
    ProjectTreeModel::~ProjectTreeModel()
    {
        delete m_Root;
    }

    /**
     * @brief ProjectTreeModel::data
     * @param index
     * @param role
     * @return
     */
    QVariant ProjectTreeModel::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid())
            return QVariant();

        if (role != Qt::DisplayRole)
            return QVariant();

        BasicTreeItem *item = static_cast<BasicTreeItem*>(index.internalPointer());

        return item->data();
    }

    /**
     * @brief ProjectTreeModel::flags
     * @param index
     * @return
     */
    Qt::ItemFlags ProjectTreeModel::flags(const QModelIndex &index) const
    {
        if (!index.isValid())
            return 0;

        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

    /**
     * @brief ProjectTreeModel::headerData
     * @param section
     * @param orientation
     * @param role
     * @return
     */
    QVariant ProjectTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        Q_UNUSED(section);

        return orientation == Qt::Horizontal && role == Qt::DisplayRole ? m_Root->data() : QVariant();
    }

    /**
     * @brief ProjectTreeModel::index
     * @param row
     * @param column
     * @param parent
     * @return
     */
    QModelIndex ProjectTreeModel::index(int row, int column, const QModelIndex &parent) const
    {
        if (!hasIndex(row, column, parent))
            return QModelIndex();

        BasicTreeItem *parentItem;

        if (!parent.isValid())
            parentItem = m_Root;
        else
            parentItem = static_cast<BasicTreeItem*>(parent.internalPointer());

        BasicTreeItem *childItem(parentItem->child(row));
        return childItem ? createIndex(row, column, childItem) : QModelIndex();
    }

    /**
     * @brief ProjectTreeModel::parent
     * @param child
     * @return
     */
    QModelIndex ProjectTreeModel::parent(const QModelIndex &child) const
    {
        if (!child.isValid())
            return QModelIndex();

        BasicTreeItem *childItem = static_cast<BasicTreeItem*>(child.internalPointer());
        BasicTreeItem *parent = childItem->parent();

        if (parent == m_Root)
            return QModelIndex();

        return createIndex(parent->row(), 0, parent);
    }

    /**
     * @brief ProjectTreeModel::rowCount
     * @param parent
     * @return
     */
    int ProjectTreeModel::rowCount(const QModelIndex &parent) const
    {
        BasicTreeItem *parentItem;
        if (parent.column() > 0)
            return 0;

        if (!parent.isValid())
            parentItem = m_Root;
        else
            parentItem = static_cast<BasicTreeItem*>(parent.internalPointer());

        return parentItem->childCount();
    }

    /**
     * @brief ProjectTreeModel::columnCount
     * @param parent
     * @return
     */
    int ProjectTreeModel::columnCount(const QModelIndex &parent) const
    {
        if (parent.isValid())
            return static_cast<BasicTreeItem*>(parent.internalPointer())->columnCount();
        else
            return m_Root->columnCount();
    }

    /**
     * @brief ProjectTreeModel::fillData
     */
    void ProjectTreeModel::fillData()
    {
        for (int i = 0; i < 10; ++i) {
            BasicTreeItem *item = new BasicTreeItem("item " + QString::number(i), m_Root);
            m_Root->appendChild(item);

            for (int j = 0; j < 3; ++j) {
                BasicTreeItem *innerItem = new BasicTreeItem("inner item " + QString::number(j), item);
                item->appendChild(innerItem);
            }
        }
    }

} // namespace models
