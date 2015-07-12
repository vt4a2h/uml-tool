/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 11/07/2015.
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

#include <QStyledItemDelegate>

#include <models/componentsmodel.h>

#include "gui_types.hpp"

namespace gui {

    class SignatureEditDelegate : public QStyledItemDelegate
    {
        Q_OBJECT

    public:
        SignatureEditDelegate(QObject *parent = nullptr);
        ~SignatureEditDelegate();

        const ComponentsMaker &maker() const;
        ComponentsMaker &maker();

    public: // QAbstractItemDelegate implementation
        QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
        void setEditorData(QWidget *editor, const QModelIndex &index) const override;
        void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    private slots:
        void onEditFinished();
        void onTextEdited();

    private:
        UniqueComponentsMaker m_ComponentsMaker;
        mutable models::DisplayPart m_DisplayPart;
    };

} // namespace gui
