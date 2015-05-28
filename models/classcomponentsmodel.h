/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 24/05/2015.
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

#include <QAbstractTableModel>

#include <entity/entity_types.hpp>

namespace models {

    enum class DisplayPart : int {
        Methods,
        Fields,
        Variables,
        Properties,
    };

    /// The ClassComponentsModel class
    class ClassComponentsModel : public QAbstractTableModel
    {
    public:
        ClassComponentsModel(const entity::SharedComponents &components, QObject *parent = nullptr);

    public: // QAbstractItemModel implementation
        int rowCount(const QModelIndex &parent) const;
        int columnCount(const QModelIndex &parent) const;
        QVariant data(const QModelIndex &index, int role) const;

        entity::SharedComponents components() const;
        void setComponents(const entity::SharedComponents &components);

    private:
        entity::SharedComponents m_Components;
        DisplayPart m_display = DisplayPart::Methods;
    };

} // namespace models
