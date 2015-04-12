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
     * @brief ProjectTreeModel::removeRows
     * @param row
     * @param count
     * @param parent
     * @return
     */
    bool ProjectTreeModel::removeRows(int row, int count, const QModelIndex &parent)
    {
        // TODO: remove children from all interval: (row, row + count)
        beginRemoveRows(parent, row, row + count);

        bool result = false;
        if (parent.isValid()) {
            BasicTreeItem *item = static_cast<BasicTreeItem*>(parent.internalPointer());
            result = item->removeChild(item->child(row));
        } else {
            if (m_Items.size() < row) {
                auto it = std::find(m_Items.begin(), m_Items.end(), m_Items.at(row));

                if (it != m_Items.end()) {
                    m_Items.erase(it, it + count);
                    result = true;
                }
            } else {
                result = false;
            }
        }

        endRemoveRows();

        return result;
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
     * @brief ProjectTreeModel::removeScope
     * @param scopeId
     * @param projectId
     */
    void ProjectTreeModel::removeScope(const QString &scopeId, const QString &projectId)
    {
        if (auto pr = find(projectId)) {
            if (auto scopeItem = pr->itemById(scopeId)) {
                int parentIndex = indexOf(pr);
                int currentIndex = pr->rowForItem(scopeItem) + parentIndex;
                auto in = index(parentIndex, 0);

                removeRows(currentIndex, currentIndex + scopeItem->childCount(), in);
            }
        }
    }

    /**
     * @brief ProjectTreeModel::addType
     * @param type
     * @param scopeId
     * @param projectId
     */
    void ProjectTreeModel::addType(const entity::SharedType &type, const QString &scopeId,
                                   const QString &projectId)
    {
        // TODO: refactore
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
     * @brief ProjectTreeModel::removeType
     * @param projectID
     * @param scopeID
     * @param typeID
     */
    void ProjectTreeModel::removeType(const QString &projectID, const QString &scopeID, const QString &typeID)
    {
        if (auto &&pr = find(projectID)) {
            if (auto &&scopeItem = pr->itemById(scopeID)) {
                int parentIndexForScope = indexOf(pr);

                if (auto &&typeItem = scopeItem->itemById(typeID)) {
                    const int parentIndexForType = scopeItem->rowForItem(typeItem);
                    const int currentIndex = pr->rowForItem(scopeItem) + parentIndexForScope + parentIndexForType;
                    auto &&in = index(parentIndexForScope, 0);

                    removeRows(currentIndex, 1, in); // TODO: imvestigate
                }
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

    /**
     * @brief ProjectTreeModel::indexOf
     * @param parent
     * @return
     */
    int ProjectTreeModel::indexOf(const BasicTreeItem *parent)
    {
        return m_Items.indexOf(*parent);
    }

    /**
     * @brief ProjectTreeModel::find
     * @param id
     * @return
     */
    BasicTreeItem *ProjectTreeModel::find(const QString &id)
    {
        return const_cast<BasicTreeItem*>(const_cast<const ProjectTreeModel*>(this)->find(id));
    }

    /**
     * @brief ProjectTreeModel::find
     * @param id
     * @return
     */
    const BasicTreeItem *ProjectTreeModel::find(const QString &id) const
    {
        auto projectIt = std::find_if(m_Items.cbegin(), m_Items.cend(),
                                      [&](const BasicTreeItem &item){
                                          return item.id() == id;
                                      });

        return projectIt != m_Items.cend() ? &*projectIt : nullptr;
    }

} // namespace models
