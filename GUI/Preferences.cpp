/*****************************************************************************
**
** Copyright (C) 2017 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 04.
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
#include "Preferences.h"
#include "ui_Preferences.h"

#include <QFileDialog>
#include <QMessageBox>

#include <Application/Settings.h>

#include <DB/Database.h>

#include <Models/ApplicationModel.h>

#include "QtHelpers.h"
#include "Constants.h"

namespace GUI {

    /**
     * @brief Preferences::Preferences
     * @param parent
     */
    Preferences::Preferences(const Models::SharedApplicationModel &appModel, QWidget *parent)
        : QDialog(parent)
        , ui(new Ui::Preferences)
        , m_AppModel(G_ASSERT(appModel))
    {
        ui->setupUi(this);
        G_CONNECT(ui->pbApply   , &QPushButton::clicked, [this]{ applyChanges();            });
        G_CONNECT(ui->pbOK      , &QPushButton::clicked, [this]{ applyChanges(); accept();  });
        G_CONNECT(ui->pbCancel  , &QPushButton::clicked, [this]{ reject();                  });
        G_CONNECT(ui->tbChooseDb, &QPushButton::clicked, [this]{ chooseNewGlobalDatabase(); });
    }

    /**
     * @brief Preferences::~Preferences
     */
    Preferences::~Preferences()
    {
    }

    /**
     * @brief Preferences::init
     */
    void Preferences::init()
    {
        // General
        ui->leGlobalDb->setText(DB::Database::mkPath(App::Settings::globalDbPath(),
                                                     App::Settings::globalDbName()));
    }

    /**
     * @brief Preferences::applyChanges
     */
    void Preferences::applyChanges()
    {
        bool changed = applyGeneralSettings(); // Add other sections through || operator
        if (changed)
            emit preferencesChanged();
    }

    /**
     * @brief Preferences::chooseNewGlobalDatabse
     */
    void Preferences::chooseNewGlobalDatabase()
    {
        auto result = QFileDialog::getOpenFileName(
                          this, tr("Choose new application database"),
                          App::Settings::globalDbPath(),
                          tr("Q-UML Project files (*.%1)").arg(DATABASE_FILE_EXTENTION)
                      );

        if (!result.isEmpty() && result.endsWith(DATABASE_FILE_EXTENTION))
            ui->leGlobalDb->setText(result);
    }

    /**
     * @brief Preferences::applyGeneralSettings
     */
    bool Preferences::applyGeneralSettings()
    {
        bool changed = false;

        // Application database
        auto db = G_ASSERT(m_AppModel->globalDatabase());
        if (db->fullPath() != ui->leGlobalDb->text()) {
            auto projects = m_AppModel->projects();
            if (!projects.isEmpty())
                QMessageBox::warning(this, tr("Some projects are opened"),
                                     tr("You cannot change global database if there are "
                                        "one or more projects opened.\nTry closing them "
                                        "and change application database after that, please."),
                                     QMessageBox::Ok);
            else {
                auto splittedPath = DB::Database::splitPath(ui->leGlobalDb->text());

                // To avoid double change
                db->setPath(splittedPath.path);
                db->setName(splittedPath.name);

                emit globalDatabaseChanged(splittedPath.path, splittedPath.name);
                changed = true;
            }
        }

        return changed;
    }

    /**
     * @brief Preferences::showEvent
     * @param event
     */
    void Preferences::showEvent(QShowEvent *event)
    {
        init();
        QDialog::showEvent(event);
    }

} // namespace GUI


