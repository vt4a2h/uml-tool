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

#include <functional>

#include <QHash>
#include <QIcon>

#include <project/project.h>
#include <entity/scope.h>
#include <entity/type.h>
#include <entity/field.h>
#include <entity/classmethod.h>
#include <relationship/relation.h>

namespace models {

    /**
     * @brief BasicTreeItem::BasicTreeItem
     * @param data
     * @param parent
     */
    BasicTreeItem::BasicTreeItem(const QVariant &data, const TreeItemType &type, BasicTreeItem *parent)
        : m_Entity(data)
        , m_Type(type)
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
        BasicTreeItem *newChild = new BasicTreeItem(data, models::TreeItemType::StubItem, this);
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

    namespace {
        QMap<TreeItemType, std::function<QVariant(const QVariant&)>> nameGetters = {
              {TreeItemType::ProjectItem,
               [](const QVariant &item){ return item.value<project::SharedProject>()->name(); }},
              {TreeItemType::ScopeItem,
               [](const QVariant &item){ return item.value<entity::SharedScope>()->name(); }},
              {TreeItemType::TypeItem,
               [](const QVariant &item){ return item.value<entity::SharedType>()->name(); }},
              {TreeItemType::FieldItem,
               [](const QVariant &item){ return item.value<entity::SharedField>()->name(); }},
              {TreeItemType::MethodItem,
               [](const QVariant &item){ return item.value<entity::SharedMethod>()->name(); }},
              {TreeItemType::RelationItem,
               [](const QVariant &item){ return item.value<relationship::SharedRelation>()->description(); }}
        };

        QMap<TreeItemType, QIcon> icons = {
            {TreeItemType::ProjectItem,  QIcon(":/icons/pic/icon_stub.png")},
            {TreeItemType::ScopeItem,    QIcon(":/icons/pic/icon_scope.png")},
            {TreeItemType::TypeItem,     QIcon(":/icons/pic/icon_class.png")},
            {TreeItemType::FieldItem,    QIcon(":/icons/pic/icon_field.png")},
            {TreeItemType::MethodItem,   QIcon(":/icons/pic/icon_method.png")},
            {TreeItemType::RelationItem, QIcon(":/icons/pic/icon_relation.png")},
            {TreeItemType::StubItem,     QIcon(":/icons/pic/icon_stub.png")}
        };
    }

    /**
     * @brief BasicTreeItem::data
     * @return
     */
    QVariant BasicTreeItem::name() const
    {
        if (m_Type == TreeItemType::StubItem)
            return tr("Stub item");

        return nameGetters[m_Type](m_Entity);
    }

    /**
     * @brief BasicTreeItem::icon
     * @return
     */
    QVariant BasicTreeItem::icon() const
    {
        return icons[m_Type];
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
