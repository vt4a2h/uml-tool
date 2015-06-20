/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 20/06/2015.
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
#include "chooseglobaldatabasedialog.h"
#include "ui_chooseglobaldatabasedialog.h"

#include <QFileDialog>
#include <QMessageBox>

#include "constants.h"

namespace gui {

    /**
     * @brief ChooseGlobalDatabaseDialog::ChooseGlobalDatabaseDialog
     * @param parent
     */
    ChooseGlobalDatabaseDialog::ChooseGlobalDatabaseDialog(QWidget *parent)
        : QDialog(parent)
        , ui(new Ui::ChooseGlobalDatabaseDialog)
        , m_Path("")
    {
        ui->setupUi(this);

        connect(ui->pbAccept, &QPushButton::clicked, [this](){ m_Path = ui->lePath->text(); accept(); });
        connect(ui->pbReject, &QPushButton::clicked, [this](){ m_Path.clear(); reject(); });
        connect(ui->tbSelectFile, &QToolButton::clicked, [this]() {
            ui->lePath->setText(QFileDialog::getOpenFileName(
                                    this, tr("Select database file."), QApplication::applicationDirPath(),
                                    QString("Qt-Uml files(.%1)").arg(DATABASE_FILE_EXTENTION)
                                ));
        } );
    }

    /**
     * @brief ChooseGlobalDatabaseDialog::~ChooseGlobalDatabaseDialog
     */
    ChooseGlobalDatabaseDialog::~ChooseGlobalDatabaseDialog()
    {
    }

    /**
     * @brief ChooseGlobalDatabaseDialog::path
     * @return
     */
    QString ChooseGlobalDatabaseDialog::path() const
    {
        return m_Path;
    }

} // namespace gui
