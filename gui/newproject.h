/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 08/01/2015.
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

namespace gui {

    namespace Ui {
        class NewProject;
    }

    /**
     * @brief The NewProject class
     */
    class NewProject : public QDialog
    {
        Q_OBJECT

    public:
        Q_DISABLE_COPY(NewProject)

        explicit NewProject(QWidget *parent = 0);
        ~NewProject();

    signals:
        void newProject(const QString &name, const QString &path);

    private slots:
        void on_btnCreateProject_clicked();
        void on_btnChooseDirectory_clicked();

    private:
        void clear();

        QScopedPointer<Ui::NewProject> ui;
    };

} // namespace gui
