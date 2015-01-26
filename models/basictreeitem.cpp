/*****************************************************************************
**
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 01/10/2015.
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
#include "basictreeitem.h"

namespace models {

    /**
     * @brief BasicTreeItem::BasicTreeItem
     * @param data
     * @param parent
     */
    BasicTreeItem::BasicTreeItem(const QVariant &data, BasicTreeItem *parent)
        : m_Entity(data)
        , m_Parent(parent)
    {
    }

    /**
     * @brief BasicTreeItem::appendChild
     * @param child
     */
    void BasicTreeItem::appendChild(BasicTreeItem * child)
    {
        m_Children.append(child);
    }

    /**
     * @brief BasicTreeItem::makeChild
     */
    BasicTreeItem *BasicTreeItem::makeChild(const QVariant &data)
    {
        BasicTreeItem *newChild = new BasicTreeItem(data, this);
        m_Children.append(newChild);

        return newChild;
    }

    /**
     * @brief BasicTreeItem::child
     * @param row
     * @return
     */
    BasicTreeItem::~BasicTreeItem()
    {
        qDeleteAll(m_Children);
    }

    BasicTreeItem * BasicTreeItem::child(int row) const
    {
        return m_Children.value(row);
    }

    /**
     * @brief BasicTreeItem::childCount
     * @return
     */
    int BasicTreeItem::childCount() const
    {
        return m_Children.count();
    }

    /**
     * @brief BasicTreeItem::columnCount
     * @return
     */
    int BasicTreeItem::columnCount() const
    {
        return 1; // only on column need; is not depends on data
    }

    /**
     * @brief BasicTreeItem::data
     * @return
     */
    QVariant BasicTreeItem::data() const
    {
        return m_Entity;
    }

    /**
     * @brief BasicTreeItem::row
     * @return
     */
    int BasicTreeItem::row() const
    {
        return m_Parent ? m_Parent->m_Children.indexOf(const_cast<BasicTreeItem*>(this)) : 0;
    }

    /**
     * @brief BasicTreeItem::parent
     * @return
     */
    BasicTreeItem *BasicTreeItem::parent() const
    {
        return m_Parent;
    }

    namespace {
        using IconPathMap = QMap<TreeItemType, QString>;
        const static IconPathMap icons = {
            {TreeItemType::ProjectItem, "project icon path"},
            {TreeItemType::TypeItem,    "type icon path"   }
        };

    }

    /**
     * @brief BasicTreeItem::iconPath
     * @return
     */
    QString BasicTreeItem::iconPath() const
    {
        return icons[m_Type];
    }

    /**
     * @brief BasicTreeItem::ItemType
     * @return
     */
    TreeItemType BasicTreeItem::itemType() const
    {
        return m_Type;
    }

    /**
     * @brief BasicTreeItem::setItemType
     * @param itemType
     */
    void BasicTreeItem::setItemType(const TreeItemType &itemType)
    {
        m_Type = itemType;
    }

} // namespace models
