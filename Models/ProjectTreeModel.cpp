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
#include "ProjectTreeModel.h"

#include <QPixmap>
#include <QFont>
#include <QDebug>

#include <range/v3/algorithm/find_if.hpp>

#include <Project/Project.h>

#include <Entity/Scope.h>
#include <Entity/Class.h>
#include <Entity/ClassMethod.h>
#include <Entity/field.h>
#include <Entity/Union.h>

#include <DB/ProjectDatabase.h>

#include <Utility/helpfunctions.h>

namespace Models {

    namespace {
        template <class ItemType>
        BasicTreeItem *addItem(const ItemType &item, BasicTreeItem *parent, TreeItemType type)
        {
            return parent->makeChild(item, type);
        }


        BasicTreeItem * itemForIndex(const QModelIndex & index)
        {
            return static_cast<BasicTreeItem*>(index.internalPointer());
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
        if (!item)
            return result;

        switch (role) {
            case Qt::DisplayRole:
                result = item->name();
                break;

            case Qt::DecorationRole:
                result = QPixmap(item->iconPath());
                break;

            case Qt::FontRole:
            {
                if (item->type() == TreeItemType::ProjectItem) {
                    if (m_CurrentProject && item->name() == m_CurrentProject->name()) {
                        QFont f;
                        f.setBold(true);
                        result = f;
                    }
                }

                break;
            }

            case SharedData:
                result = item->entity();
                break;

            case DataType:
                result = QVariant::fromValue(item->type());
                break;

            case ID:
                result = QVariant::fromValue(item->id());
                break;

            default: ;
        }

        return result;
    }

    template<class UnderlyingType>
    bool setDataImpl(const BasicTreeItem & item, const QVariant & value,
                     std::function<void()> postFunc)
    {
        Q_ASSERT(item.entity().canConvert<UnderlyingType>());

        Q_ASSERT(value.canConvert<QString>());
        QString newName = value.toString();

        UnderlyingType type = G_ASSERT(item.entity().value<UnderlyingType>());
        if (type->name() != newName && !newName.isEmpty()) {
            type->setName(newName);

            postFunc();

            return true;
        }

        return false;
    }

    /**
     * @brief ProjectTreeModel::setData
     * @param index
     * @param value
     * @param role
     * @return
     */
    bool ProjectTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
    {
        if (index.isValid() && role == Qt::EditRole) {
            BasicTreeItem * item = G_ASSERT(itemForIndex(index));

            // TODO: extend
            switch (item->type()) {
                case TreeItemType::TypeItem:
                    return setDataImpl<Entity::SharedType>(
                              *item, value, [&]{ emit dataChanged(index, index); });

//                case TreeItemType::ProjectItem:
//                    return setDataImpl<Projects::SharedProject>(
//                              *item, value, [&]{ emit dataChanged(index, index); });

            default: ;
            }
        }

        return QAbstractItemModel::setData(index, value, role);
    }

    /**
     * @brief ProjectTreeModel::flags
     * @param index
     * @return
     */
    Qt::ItemFlags ProjectTreeModel::flags(const QModelIndex &index) const
    {
        if (!index.isValid())
            return Qt::NoItemFlags;

        auto flags = QAbstractItemModel::flags(index);

        // TODO: extend
        static const QSet<TreeItemType> editingTypes = {
            TreeItemType::TypeItem/*, TreeItemType::ProjectItem*/
        };
        if (auto item = itemForIndex(index); item && editingTypes.contains(item->type()))
            flags |= Qt::ItemIsEditable;

        return flags;
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
            parentItem ? parentItem->child(row) : &m_Projects[row];

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

        return !parent.isValid() ? m_Projects.count()
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
        beginRemoveRows(parent, row, row + count);

        bool result = false;
        if (parent.isValid()) {
            BasicTreeItem *item = static_cast<BasicTreeItem*>(parent.internalPointer());
            for (int i = row; i <= row + count; ++i)
                result = item->removeChild(item->child(i));
        } else {
            if (m_Projects.size() < row) {
                auto it = std::find(m_Projects.begin(), m_Projects.end(), m_Projects.at(row));

                if (it != m_Projects.end()) {
                    m_Projects.erase(it, it + count);
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
    void ProjectTreeModel::addProject(const Projects::SharedProject &pr)
    {
        addProjectItem(pr);
    }

    /**
     * @brief ProjectTreeModel::removeProject
     * @param pr
     */
    void ProjectTreeModel::removeProject(const Projects::SharedProject &pr)
    {
        if (pr) {
            auto it = ranges::find_if(
                m_Projects, [&](auto &&p) { return p.id().toString() == pr->name(); });
            if (it != m_Projects.end()) {
                auto index = m_Projects.indexOf(*it);
                beginRemoveRows(QModelIndex(), index, index);

                m_Projects.erase(it);

                endRemoveRows();
            }
        }
    }

    /**
     * @brief ProjectTreeModel::addScope
     * @param scope
     * @param parent
     */
    void ProjectTreeModel::addScope(const Entity::SharedScope &scope, const QString &projectName)
    {
        if (auto &&pr = find(projectName)) {
            auto &&projectIndex = index(indexOf(pr), 0);
            Q_ASSERT(projectIndex.isValid());

            auto pos = projectIndex.row() + 1;
            beginInsertRows(projectIndex, pos, pos);
            auto &&newItem = pr->makeChild(QVariant::fromValue(scope), TreeItemType::ScopeItem);
            endInsertRows();

            observeItemChanging(m_CurrentProject, scope.get(), newItem);
        }
    }

    /**
     * @brief ProjectTreeModel::removeScope
     * @param scopeId
     * @param projectId
     */
    void ProjectTreeModel::removeScope(const Common::ID &scopeId, const QString &projectName)
    {
        if (auto &&pr = find(projectName)) {
            auto &&projectIndex = index(indexOf(pr), 0);
            Q_ASSERT(projectIndex.isValid());

            if (auto &&scope = pr->itemById(QVariant::fromValue(scopeId))) {
                auto &&scopeIndex = projectIndex.child(pr->rowForItem(scope), 0);
                Q_ASSERT(scopeIndex.isValid());

                removeRows(scopeIndex.row(), 0, projectIndex);
            }
        }
    }

    /**
     * @brief ProjectTreeModel::addType
     * @param type
     * @param scopeId
     * @param projectId
     */
    void ProjectTreeModel::addType(const Entity::SharedType &type, const Common::ID &scopeID,
                                   const QString &projectName)
    {
        if (auto &&pr = find(projectName)) {
            auto &&projectIndex = index(indexOf(pr), 0);
            Q_ASSERT(projectIndex.isValid());

            if (auto &&scope = pr->itemById(QVariant::fromValue(scopeID))) {
                auto &&scopeIndex = projectIndex.child(pr->rowForItem(scope), 0);
                Q_ASSERT(scopeIndex.isValid());

                auto pos = scopeIndex.row() + 1;
                beginInsertRows(scopeIndex, pos, pos);
                auto &&newItem = scope->makeChild(QVariant::fromValue(type), TreeItemType::TypeItem);
                endInsertRows();

                observeItemChanging(m_CurrentProject, type.get(), newItem);
            }
        }
    }

    /**
     * @brief ProjectTreeModel::removeType
     * @param projectID
     * @param scopeID
     * @param typeID
     */
    void ProjectTreeModel::removeType(const QString &projectName, const Common::ID &scopeID,
                                      const Common::ID &typeID)
    {
        // TODO: make universal remove function
        if (auto &&pr = find(projectName)) {
            auto &&projectIndex = index(indexOf(pr), 0);
            Q_ASSERT(projectIndex.isValid());

            if (auto &&scope = pr->itemById(QVariant::fromValue(scopeID))) {
                auto &&scopeIndex = projectIndex.child(pr->rowForItem(scope), 0);
                Q_ASSERT(scopeIndex.isValid());

                if (auto &&type = scope->itemById(QVariant::fromValue(typeID))) {
                    auto &&typeIndex = scopeIndex.child(scope->rowForItem(type), 0);
                    Q_ASSERT(typeIndex.isValid());

                    removeRow(typeIndex.row(), scopeIndex);
                }
            }
        }
    }

    /**
     * @brief ProjectTreeModel::onCurrentProjectChanged
     * @param previous
     * @param current
     */
    void ProjectTreeModel::onCurrentProjectChanged(const Projects::SharedProject &/*previous*/,
                                                   const Projects::SharedProject &current)
    {
        m_CurrentProject = current;
    }

    /**
     * @brief ProjectTreeModel::addProjectItem
     * @param pr
     */
    void ProjectTreeModel::addProjectItem(const Projects::SharedProject &pr)
    {
        beginInsertRows(QModelIndex(), rowCount(), rowCount() + 1);
        m_Projects << BasicTreeItem(QVariant::fromValue(pr), TreeItemType::ProjectItem);
        endInsertRows();

        DB::SharedProjectDatabase database = pr->database();
        auto projectItem = &m_Projects.last();
        for (auto &&scope : database->scopes()) {
            auto scopeItem = addItem(QVariant::fromValue(scope), projectItem, TreeItemType::ScopeItem);
            observeItemChanging(pr, scope.get(), scopeItem);

            for (auto &&type : scope->types()) {
                auto typeItem = addItem(QVariant::fromValue(type), scopeItem, TreeItemType::TypeItem);
                observeItemChanging(pr, type.get(), typeItem);

                for (auto &&field : type->fields()) {
                    auto fieldItem = addItem(QVariant::fromValue(field), typeItem, TreeItemType::FieldItem);
                    observeItemChanging(pr, field.get(), fieldItem);
                }

                for (auto &&method : type->methods()) {
                    auto methodItem = addItem(QVariant::fromValue(method), typeItem, TreeItemType::MethodItem);
                    observeItemChanging(pr, method.get(), methodItem);
                }
            }
        }

        connect(pr.get(), &Projects::Project::nameChanged, [=]{ update(projectItem); });
    }

    /**
     * @brief ProjectTreeModel::indexOf
     * @param parent
     * @return
     */
    int ProjectTreeModel::indexOf(const BasicTreeItem *parent)
    {
        return m_Projects.indexOf(*parent);
    }

    /**
     * @brief ProjectTreeModel::find
     * @param id
     * @return
     */
    BasicTreeItem *ProjectTreeModel::find(const QVariant &id)
    {
        return const_cast<BasicTreeItem*>(const_cast<const ProjectTreeModel*>(this)->find(id));
    }

    /**
     * @brief ProjectTreeModel::find
     * @param id
     * @return
     */
    const BasicTreeItem *ProjectTreeModel::find(const QVariant &id) const
    {
        auto projectIt = ranges::find_if(m_Projects, [&](auto &&item){ return item.id() == id; });
        return projectIt != m_Projects.cend() ? &*projectIt : nullptr;
    }

    /**
     * @brief ProjectTreeModel::update
     * @param item
     */
    void ProjectTreeModel::update(BasicTreeItem *item)
    {
        Q_ASSERT(item);
        auto parent = item; // can be Project
        QList<BasicTreeItem *> items;
        items.push_front(parent);
        while (parent->parentNode()) {
            parent = parent->parentNode();
            items.push_front(parent);
        }

        Q_ASSERT(parent);
        auto topLeftIndex = index(indexOf(parent), 0);
        Q_ASSERT(topLeftIndex.isValid());

        // update project name
        if (parent == item) {
            emit dataChanged(topLeftIndex, topLeftIndex);
            return;
        }

        auto bottomRightIndex = topLeftIndex;
        auto currentItem = items.first();
        for (int i = 1, count = items.count(); i < count; ++i) {
            bottomRightIndex = index(currentItem->rowForItem(items[i]), 0, bottomRightIndex);
            Q_ASSERT(bottomRightIndex.isValid());
            currentItem = items[i];
        }

        emit dataChanged(topLeftIndex, bottomRightIndex);
    }

    void ProjectTreeModel::observeItemChanging(const Projects::SharedProject &p,
                                               Common::BasicElement * entity, BasicTreeItem *item)
    {
        connect(entity, &Common::BasicElement::nameChanged, [=]{ update(item); });
        connect(entity, &Common::BasicElement::nameChanged,
                G_ASSERT(p.get()), &Projects::Project::touch);
    }

} // namespace models
