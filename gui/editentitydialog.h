/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 18/03/2015.
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

#include <QDialog>

#include <project/project_types.hpp>

#include <entity/entity_types.hpp>

#include <models/models_types.hpp>

class QListWidgetItem;

namespace gui
{

    namespace Ui {
        class EditEntityDialog;
    }

    /// The EditEntityDialog class
    class EditEntityDialog : public QDialog
    {
        Q_OBJECT

    public:
        Q_DISABLE_COPY(EditEntityDialog)

        explicit EditEntityDialog(QWidget *parent = 0);
        ~EditEntityDialog();

        // Call before show dialog
        void setData(const models::SharedApplicationModal &model,
                     const entity::SharedType &type);

    signals:
        void needNewScope();

    public slots:
        void onScopeAdded(const entity::SharedScope &scope);

    private slots:
        void onAccepted();
        void onRejected();
        void onNewComponentClicked();
        void onCurrentItemChange(QListWidgetItem *current, QListWidgetItem *previous);
        void onEditComponentClicked(const QModelIndex &index);
        void onDeleteComponentClicked(const QModelIndex &index);

    protected:
        void showEvent(QShowEvent *ev);
        void closeEvent(QCloseEvent *ev);

    private:
        void init();
        void clear();
        void setType();
        void setScope();

        QScopedPointer<Ui::EditEntityDialog> ui;

        entity::SharedType  m_Type;
        entity::SharedScope m_Scope;

        project::SharedProject m_Project;

        models::SharedApplicationModal m_ApplicationModel;
        models::UniqueClassComponentsModel m_ComponentsModel;
    };

} // namespace gui
