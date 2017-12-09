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
#pragma once

#include <QAbstractItemModel>

#include <Entity/entity_types.hpp>
#include <Project/ProjectTypes.hpp>
#include <Common/common_types.h>

#include "BasicTreeItem.h"

namespace Models {

    class ProjectTreeModel : public QAbstractItemModel
    {
        Q_OBJECT

    public:
        enum CustomRoles { SharedData = 0x0100, DataType, ID };

        ProjectTreeModel(QObject * parent = nullptr);

        QVariant data(const QModelIndex &index, int role) const override;
        bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;

        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

        QModelIndex parent(const QModelIndex &child) const override;

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

        void addProject(const Projects::SharedProject &pr);
        void removeProject(const Projects::SharedProject &pr);

        void addScope(const Entity::SharedScope &scope, const QString &projectName);
        void removeScope(const Common::ID &scopeId, const QString &projectName);

        void addType(const Entity::SharedType &type, const Common::ID &scopeID,
                     const QString &projectName);
        void removeType(const QString &projectName, const Common::ID &scopeID,
                        const Common::ID &typeID);

    public slots:
        void onCurrentProjectChanged(const Projects::SharedProject &previous,
                                     const Projects::SharedProject &current);

    private:
        void addProjectItem(const Projects::SharedProject &pr);
        int indexOf(const BasicTreeItem *parent);
        BasicTreeItem *find(const QVariant &id);
        const BasicTreeItem *find(const QVariant &id) const;
        void update(BasicTreeItem *item);
        void observeItemChanging(const Projects::SharedProject &p, Common::BasicElement * entity, BasicTreeItem *item);

        mutable QList<BasicTreeItem> m_Projects; // TODO: store as pointers
        Projects::SharedProject m_CurrentProject;
    };

} // namespace models
