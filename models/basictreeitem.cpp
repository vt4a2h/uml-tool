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
#include <project/project_types.hpp>

#include <entity/scope.h>
#include <entity/type.h>
#include <entity/class.h>
#include <entity/field.h>
#include <entity/classmethod.h>
#include <entity/entity_types.hpp>

#include <relationship/relation.h>
#include <relationship/relationship_types.hpp>

#include <utility/helpfunctions.h>

// NOTE: project name must be unique. It's used as ID.
#define declare_methods_map(map_name, method_name)\
    QMap<TreeItemType, std::function<QVariant(const QVariant&)>> map_name = {\
          {TreeItemType::ProjectItem,\
           [](const QVariant &item){ return QVariant::fromValue(item.value<project::SharedProject>()->name()); }},\
          {TreeItemType::ScopeItem,\
           [](const QVariant &item){ return QVariant::fromValue(item.value<entity::SharedScope>()->method_name()); }},\
          {TreeItemType::TypeItem,\
           [](const QVariant &item){ return QVariant::fromValue(item.value<entity::SharedType>()->method_name()); }},\
          {TreeItemType::FieldItem,\
           [](const QVariant &item){ return QVariant::fromValue(item.value<entity::SharedField>()->method_name()); }},\
          {TreeItemType::MethodItem,\
           [](const QVariant &item){ return QVariant::fromValue(item.value<entity::SharedMethod>()->method_name()); }},\
          {TreeItemType::RelationItem,\
           [](const QVariant &item){ return QVariant::fromValue(item.value<relationship::SharedRelation>()->method_name()); }}\
    };

namespace models {

    /**
     * @brief BasicTreeItem::BasicTreeItem
     * @param data
     * @param parent
     */
    BasicTreeItem::BasicTreeItem(const QVariant &entity, const TreeItemType &type,
                                 BasicTreeItem *parentNode, QObject *parent)
        : QObject(parent)
        , m_Entity(entity)
        , m_Type(type)
        , m_Parent(parentNode)
    {
    }

    /**
     * @brief BasicTreeItem::BasicTreeItem
     * @param src
     */
    BasicTreeItem::BasicTreeItem(const BasicTreeItem &src)
        : QObject(nullptr)
    {
        copyFrom(src);
    }

    /**
     * @brief BasicTreeItem::BasicTreeItem
     * @param src
     */
    BasicTreeItem::BasicTreeItem(BasicTreeItem &&src)
    {
        moveFrom(src);
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
    BasicTreeItem *BasicTreeItem::makeChild(const QVariant &entity, const TreeItemType &type)
    {
        BasicTreeItem *newChild = new BasicTreeItem(entity, type, this);
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

    /**
     * @brief BasicTreeItem::operator =
     * @param rhs
     * @return
     */
    BasicTreeItem &BasicTreeItem::operator =(BasicTreeItem rhs)
    {
        moveFrom(rhs);

        return *this;
    }

    /**
     * @brief BasicTreeItem::operator =
     * @param rhs
     * @return
     */
    BasicTreeItem &BasicTreeItem::operator =(BasicTreeItem &&rhs)
    {
        if (this != &rhs)
            moveFrom(rhs);

        return *this;
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const BasicTreeItem &lhs, const BasicTreeItem &rhs)
    {
        return std::equal(lhs.m_Children.begin(), lhs.m_Children.end(), rhs.m_Children.begin(),
                          [](BasicTreeItem const * l, BasicTreeItem const * r) {
                             return l == r || (l && r && *l == *r);
                          }) &&
                lhs.m_Entity == rhs.m_Entity &&
                lhs.m_Type   == rhs.m_Type   &&
                lhs.m_Parent == rhs.m_Parent;
    }

    /**
     * @brief BasicTreeItem::child
     * @param row
     * @return
     */
    BasicTreeItem * BasicTreeItem::child(int row) const
    {
        return m_Children.value(row);
    }

    /**
     * @brief BasicTreeItem::itemById
     * @param id
     * @return
     */
    BasicTreeItem *BasicTreeItem::itemById(const QVariant &id) const
    {
        for(auto &&item : m_Children)
            if (item->id() == id)
                return item;

        return nullptr;
    }

    /**
     * @brief BasicTreeItem::removeChild
     * @param child
     * @return
     */
    bool BasicTreeItem::removeChild(BasicTreeItem *child)
    {
        return m_Children.removeOne(child);
    }

    /**
     * @brief BasicTreeItem::rowForItem
     * @param item
     * @return
     */
    int BasicTreeItem::rowForItem(BasicTreeItem *item) const
    {
        return m_Children.indexOf(item);
    }

    /**
     * @brief BasicTreeItem::children
     * @return
     */
    ChildItems BasicTreeItem::childrenItems() const
    {
        return m_Children;
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
        return maxColumnCount; // only on column need; is not depends on data
    }

    namespace {
        declare_methods_map(nameGetters, name)
        declare_methods_map(idGetters, id)

        QMap<TreeItemType, QString> icons = {
            {TreeItemType::ProjectItem,  ":/icons/pic/icon_project.png" },
            {TreeItemType::ScopeItem,    ":/icons/pic/icon_scope.png"   },
            {TreeItemType::TypeItem,     ":/icons/pic/icon_class.png"   },
            {TreeItemType::FieldItem,    ":/icons/pic/icon_field.png"   },
            {TreeItemType::MethodItem,   ":/icons/pic/icon_method.png"  },
            {TreeItemType::RelationItem, ":/icons/pic/icon_relation.png"},
            {TreeItemType::StubItem,     ":/icons/pic/icon_stub.png"    }
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
    QString BasicTreeItem::iconPath() const
    {
        return icons[m_Type];
    }

    /**
     * @brief models::BasicTreeItem::entity
     * @return
     */
    QVariant BasicTreeItem::entity() const
    {
        return m_Entity;
    }

    /**
     * @brief BasicTreeItem::id
     * @return
     */
    QVariant BasicTreeItem::id() const
    {
        return m_Type != TreeItemType::StubItem ? idGetters[m_Type](m_Entity) : QVariant();
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
    BasicTreeItem *BasicTreeItem::parentNode() const
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

    /**
     * @brief BasicTreeItem::moveFrom
     * @param src
     */
    void BasicTreeItem::moveFrom(BasicTreeItem &src)
    {
        m_Children = std::move(src.m_Children);
        m_Entity   = std::move(src.m_Entity);
        m_Type     = std::move(src.m_Type);
        m_Parent   = std::move(src.m_Parent);
    }

    /**
     * @brief BasicTreeItem::copyFrom
     * @param src
     */
    void BasicTreeItem::copyFrom(const BasicTreeItem &src)
    {
        utility::deepCopyPointerList(src.m_Children, m_Children);
        m_Entity = src.m_Entity;
        m_Type   = src.m_Type;
        m_Parent = src.m_Parent;
    }

} // namespace models
