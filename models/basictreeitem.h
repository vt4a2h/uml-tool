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
#pragma once

#include <memory>

#include <QString>
#include <QVector>
#include <QMap>
#include <QVariant>
#include <QCoreApplication>

#include <entity/entityid.h>

#include "models_types.hpp"

/// Namespace models
namespace models {

    /// The TreeItemType enum
    enum class TreeItemType : int {
        StubItem,    ///< StubItem
        ProjectItem, ///< ProjectItem
        ScopeItem,   ///< ScopeItem
        TypeItem,    ///< TypeItem
        FieldItem,   ///< FieldItem
        MethodItem,  ///< MethodItem
        RelationItem ///< RelationItem
    };

    /// The BasicTreeItem class
    class BasicTreeItem : public QObject
    {
        Q_OBJECT

    public:
        BasicTreeItem(const QVariant &entity, const TreeItemType &type,
                      BasicTreeItem *parentNode = nullptr, QObject * parent = nullptr);
        BasicTreeItem(const BasicTreeItem &src);
        BasicTreeItem(BasicTreeItem &&src);
        ~BasicTreeItem();

        BasicTreeItem &operator =(BasicTreeItem rhs);
        BasicTreeItem &operator =(BasicTreeItem &&rhs);
        friend bool operator ==(const BasicTreeItem &lhs, const BasicTreeItem &rhs);

        void appendChild(BasicTreeItem *child);
        BasicTreeItem *makeChild(const QVariant &entity, const TreeItemType &type);
        BasicTreeItem *child(int row) const;
        BasicTreeItem *itemById(const QVariant &id) const;
        bool removeChild(BasicTreeItem * child);
        int rowForItem(BasicTreeItem * item) const;
        ChildItems childrenItems() const;

        int childCount() const;
        int columnCount() const;

        QVariant name() const;
        QString iconPath() const;

        QVariant entity() const;

        QVariant id() const;

        int row() const;

        BasicTreeItem *parentNode() const;

        TreeItemType itemType() const;
        void setItemType(const TreeItemType &itemType);

        const static int maxColumnCount = 1;

    private:
        void moveFrom(BasicTreeItem &src);
        void copyFrom(const BasicTreeItem &src);

        ChildItems m_Children;

        QVariant m_Entity;
        TreeItemType m_Type;

        BasicTreeItem *m_Parent;
    };

} // namespace models

Q_DECLARE_METATYPE(models::TreeItemType)
