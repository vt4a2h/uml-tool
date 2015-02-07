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

#include "types.h"

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
    class BasicTreeItem
    {
        Q_DECLARE_TR_FUNCTIONS(BasicTreeItem)

    public:
        BasicTreeItem(const QVariant &entity, const TreeItemType &type, BasicTreeItem *parent = nullptr);
        ~BasicTreeItem();

        void appendChild(BasicTreeItem *child);
        BasicTreeItem *makeChild(const QVariant &entity, const TreeItemType &type);
        BasicTreeItem *child(int row) const;

        int childCount() const;
        int columnCount() const;

        QVariant name() const;
        QString iconPath() const;

        int row() const;

        BasicTreeItem *parent() const;

        TreeItemType itemType() const;
        void setItemType(const TreeItemType &itemType);

        const static int maxColumnCount = 1;

    private:
        ChildItems m_Children;

        QVariant m_Entity;
        TreeItemType m_Type;

        BasicTreeItem *m_Parent;
    };

} // namespace models
