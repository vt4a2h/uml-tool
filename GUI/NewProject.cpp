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
#include "NewProject.h"
#include "ui_NewProject.h"

#include <QFileDialog>
#include <QMessageBox>

#include <Application/Settings.h>

namespace GUI {

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
        QFileInfo path(ui->editProjectPath->text());
        QString name = ui->editProjectName->text().simplified().replace(QChar::Space, "_");

        if (!checkName(name) || !checkPath(path))
            return;

        emit newProject(name, path.absoluteFilePath());
        App::Settings::setLastNewProjectDir(path.absoluteFilePath());

        clear();
        accept();
    }

    /**
     * @brief NewProject::on_btnChooseDirectory_clicked
     */
    void NewProjectDialog::on_btnChooseDirectory_clicked()
    {
        QString result = QFileDialog::getExistingDirectory(this, tr("Choose project directory"),
                                                           App::Settings::lastNewProjectDir());

        if (!result.isEmpty()) {
            ui->editProjectPath->setText(result);
            App::Settings::setLastNewProjectDir(result);
        }

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

    /**
     * @brief NewProjectDialog::checkPath
     * @param path
     * @return
     */
    bool NewProjectDialog::checkPath(const QFileInfo &path)
    {
        if (!path.exists()) {
            if (QMessageBox::question(this, tr("Invalid path"),
                                      tr("The path \"%1\" is not exists.\nDo you want to create it?")
                                      .arg(path.absoluteFilePath()),
                                      QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
                QDir().mkdir(path.absoluteFilePath());
            }
        }

        if (!path.exists()) {
            QMessageBox::warning(this, tr("Invalid path"), tr("Directory is not exists."),
                                 QMessageBox::Ok);
            return false;
        }

        return true;
    }

    /**
     * @brief NewProjectDialog::checkName
     * @param name
     * @return
     */
    bool NewProjectDialog::checkName(const QString &name)
    {
        if (name.isEmpty())
            QMessageBox::warning(this, tr( "Invalid name" ), tr("Name is empty."), QMessageBox::Ok);

        return !name.isEmpty();
    }

} // namespace gui

