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
#include <QVariant>

namespace models {

    class BasicTreeItem;

    // TODO: just for tests now; move to types header
    using ChildItems = QVector<BasicTreeItem*>;

    class BasicTreeItem
    {
    public:
        BasicTreeItem(const QString &data, BasicTreeItem *parent = nullptr);
        ~BasicTreeItem();

        void appendChild(BasicTreeItem *child);
        BasicTreeItem *makeChild(const QString & data = QString());
        BasicTreeItem *child(int row) const;

        int childCount() const;
        int columnCount() const;

        QVariant data() const;

        int row() const;

        BasicTreeItem *parent() const;

    private:
        ChildItems m_Children;
        const QString m_Data; // NOTE: just for test, will be more complicated
        BasicTreeItem * m_Parent;
    };

} // namespace models
