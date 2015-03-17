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

#include <QPixmap>
#include <QDebug>

#include <project/project.h>

#include <entity/scope.h>
#include <entity/class.h>
#include <entity/classmethod.h>
#include <entity/field.h>

#include <db/projectdatabase.h>

namespace models {

    namespace {
        template <class ItemType>
        BasicTreeItem *addItem(const ItemType &item, BasicTreeItem *parent, TreeItemType type)
        {
            return parent->makeChild(item, type);
        }
    }

    /**
     * @brief ProjectTreeModel::ProjectTreeModel
     * @param parent
     */
    ProjectTreeModel::ProjectTreeModel(QObject *parent)
        : QAbstractItemModel(parent)
    {
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

        QVariant result;
        BasicTreeItem *item = static_cast<BasicTreeItem*>(index.internalPointer());

        switch (role) {
            case Qt::DisplayRole:
                result = item->name();
                break;

            case Qt::DecorationRole:
                result = QPixmap(item->iconPath());
                break;

            case SharedData:
                result = item->entity();
                break;

            case DataType:
                result = QVariant::fromValue(item->itemType());
                break;

            case ID:
                result = QVariant::fromValue(item->id());
                break;

            default: ;
        }

        return result;
    }

    /**
     * @brief ProjectTreeModel::flags
     * @param index
     * @return
     */
    Qt::ItemFlags ProjectTreeModel::flags(const QModelIndex &index) const
    {
        return index.isValid() ? QAbstractItemModel::flags(index) : Qt::NoItemFlags;
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

        return orientation == Qt::Horizontal && role == Qt::DisplayRole ? "stub" : QVariant();
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

        BasicTreeItem *parentItem =
            parent.isValid() ? static_cast<BasicTreeItem*>(parent.internalPointer()) : nullptr;
        BasicTreeItem *childItem  =
            parentItem ? parentItem->child(row) : &m_Items[row];

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
        BasicTreeItem *parentItem = childItem->parentNode();

        return parentItem ? createIndex(parentItem->row(), 0, parentItem) : QModelIndex();
    }

    /**
     * @brief ProjectTreeModel::rowCount
     * @param parent
     * @return
     */
    int ProjectTreeModel::rowCount(const QModelIndex &parent) const
    {
        if (parent.column() > 0)
            return 0;

        return !parent.isValid() ? m_Items.count()
                                 : static_cast<BasicTreeItem*>(parent.internalPointer())->childCount();
    }

    /**
     * @brief ProjectTreeModel::columnCount
     * @param parent
     * @return
     */
    int ProjectTreeModel::columnCount(const QModelIndex &parent) const
    {
        return parent.isValid() ? static_cast<BasicTreeItem*>(parent.internalPointer())->columnCount()
                                : BasicTreeItem::maxColumnCount;
    }

    /**
     * @brief ProjectTreeModel::addProject
     * @param pr
     */
    void ProjectTreeModel::addProject(const project::SharedProject &pr)
    {
        addProjectItem(pr);
    }

    /**
     * @brief ProjectTreeModel::addScope
     * @param scope
     * @param parent
     */
    void ProjectTreeModel::addScope(const entity::SharedScope &scope, const QString &projectId)
    {
        auto it = std::find_if(m_Items.begin(), m_Items.end(),
                               [&](const BasicTreeItem &item){ return item.id() == projectId; });
        if (it != m_Items.end()) {
            int parentIndex = std::distance(m_Items.begin(), it);
            if (parentIndex == 0)
                ++parentIndex;
            beginInsertRows(index(parentIndex, 0), parentIndex, parentIndex + 1);
            it->makeChild(QVariant::fromValue(scope), TreeItemType::ScopeItem);
            endInsertRows();
        }
    }

    /**
     * @brief ProjectTreeModel::addType
     * @param type
     * @param scopeId
     * @param projectId
     */
    void ProjectTreeModel::addType(const entity::SharedType &type, const QString &scopeId, const QString &projectId)
    {
        auto projectIt = std::find_if(m_Items.begin(), m_Items.end(),
                                      [&](const BasicTreeItem &item){ return item.id() == projectId; });

        if (projectIt != m_Items.end()) {
            auto projectIndex = std::distance(m_Items.begin(), projectIt);

            auto scopes = projectIt->childrenItems();
            auto scopeIt = std::find_if(scopes.begin(), scopes.end(),
                                        [&](const BasicTreeItem *item){ return item->id() == scopeId; });
            if (scopeIt != scopes.end()) {
                int parentIndex = std::distance(scopes.begin(), scopeIt);
                parentIndex += projectIndex;
                beginInsertRows(index(parentIndex, 0), parentIndex, parentIndex + 1);
                (*scopeIt)->makeChild(QVariant::fromValue(type), TreeItemType::TypeItem);
                endInsertRows();
            }
        }
    }

    /**
     * @brief ProjectTreeModel::addProjectItem
     * @param pr
     */
    void ProjectTreeModel::addProjectItem(const project::SharedProject &pr)
    {
        beginInsertRows(QModelIndex(), rowCount(), rowCount() + 1);
        m_Items << BasicTreeItem(QVariant::fromValue(pr), TreeItemType::ProjectItem);
        endInsertRows();

        // TODO: add beginInsertRows endInsertRows for all items

        db::SharedProjectDatabase database = pr->database();
        auto projectItem = &m_Items.last();
        for (auto &&scope : database->scopes()) {
            auto scopeItem =
                addItem(QVariant::fromValue(scope), projectItem, TreeItemType::ScopeItem);

            for (auto &&type : scope->types()) {
                auto typeItem =
                    addItem(QVariant::fromValue(type), scopeItem, TreeItemType::TypeItem);

                for (auto &&field : type->fields())
                    addItem(QVariant::fromValue(field), typeItem, TreeItemType::FieldItem);
                for (auto &&method : type->methods())
                    addItem(QVariant::fromValue(method), typeItem, TreeItemType::MethodItem);
            }
        }
    }

} // namespace models
