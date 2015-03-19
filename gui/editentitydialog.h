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

    private:
        QScopedPointer<Ui::EditEntityDialog> ui;
    };

} // namespace gui