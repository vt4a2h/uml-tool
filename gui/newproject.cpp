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
#include "newproject.h"
#include "ui_newproject.h"

#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>

namespace gui {

    /**
     * @brief NewProject::NewProject
     * @param parent
     */
    NewProjectDialog::NewProjectDialog(QWidget *parent)
        : QDialog(parent)
        , ui(new Ui::NewProject)
    {
        ui->setupUi(this);
    }

    /**
     * @brief NewProject::~NewProject
     */
    NewProjectDialog::~NewProjectDialog()
    {
    }

    /**
     * @brief NewProject::on_btnCreateProject_clicked
     */
    void NewProjectDialog::on_btnCreateProject_clicked()
    {
        QString path = ui->editProjectPath->text();
        QString name = ui->editProjectName->text().simplified().replace( " ", "_" );

        if (!QFileInfo(path).exists()) {
            // TODO: ask about creation path
            QMessageBox::warning(this, tr( "Invalid path" ), tr("Directory is not exists."), QMessageBox::Ok);
            return;
        }

        if (name.isEmpty()) {
            QMessageBox::warning(this, tr( "Invalid name" ), tr("Name is empty."), QMessageBox::Ok);
            return;
        }

        emit newProject(name, path);
        clear();
        reject();
    }

    /**
     * @brief NewProject::on_btnChooseDirectory_clicked
     */
    void NewProjectDialog::on_btnChooseDirectory_clicked()
    {
        QString result = QFileDialog::getExistingDirectory(this, tr("Choose project directory"));

        if (!result.isEmpty())
            ui->editProjectPath->setText(result);

        ui->editProjectPath->setFocus();
    }

    /**
     * @brief NewProject::clear
     */
    void NewProjectDialog::clear()
    {
        ui->editProjectName->clear();
        ui->editProjectPath->clear();
    }

} // namespace gui

