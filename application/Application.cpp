/*****************************************************************************
**
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 03/11/2014.
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
#include "Application.h"

#include <QMessageBox>
#include <QApplication>
#include <QFile>

#include <gui/MainWindow.h>
#include <gui/chooseglobaldatabasedialog.h>

#include <models/applicationmodel.h>

#include <helpers/generatorid.h>

#include <entity/Type.h>
#include <entity/EntityFactory.h>

#include <relationship/RelationFactory.h>

#include "settings.h"
#include "qthelpers.h"

namespace application {

    /**
     * @brief Application::run
     */
    bool Application::run()
    {
        m_MainWindow->show();

        auto db = G_ASSERT(m_ApplicationModel->globalDatabase());
        db->setPath(settings::globalDbPath());
        db->setName(settings::globalDbName());

        QStringList errors;
        db->load(errors);

        db->save();

        if (!errors.isEmpty()) {
            QMessageBox::warning(m_MainWindow.get(), tr("Problems with loading global database"), errors.join("\n"),
                                 QMessageBox::Ok);
            db->clear();
            errors.clear();

            gui::ChooseGlobalDatabaseDialog dlg(db->name(), db->path(), m_MainWindow.get());
            if (dlg.exec() == QDialog::Accepted)
            {
                db->setName(dlg.name());
                db->setPath(dlg.path());

                if (QFile::exists(db->fullPath())) {
                    db->load(errors);

                    if (!errors.isEmpty()) {
                        QMessageBox::critical(m_MainWindow.get(), tr("Database reading error"), errors.join("\n"),
                                              QMessageBox::Ok);
                        return false;
                    }
                } else {
                    // Create new database
                    if (!db->save()) {
                        QMessageBox::critical(m_MainWindow.get(),
                                              tr("Database creating error"),
                                              tr("Cannot create new database: %1.").arg(db->fullPath()),
                                              QMessageBox::Ok);
                        return false;
                    }
                }

                settings::setGlobalDbPath(db->path());
                settings::setGlobalDbName(db->name());
            } else {
                QMessageBox::information(m_MainWindow.get(), tr("Ohh"), tr("You cannot run application without database!"),
                                         QMessageBox::Ok );
                return false;
            }
        }

        return true;
    }

    namespace {

        template <class Factory>
        void setUpFactory(const Factory& f, const models::SharedApplicationModel &model,
                          const QPointer<QGraphicsScene>& scene)
        {
            G_CONNECT(model.get(), &models::ApplicationModel::currentProjectChanged,
                      &f, &Factory::onProjectChanged);

            auto &factory = const_cast<Factory&>(f);
            factory.onSceneChanged(scene);
            factory.setGlobalDatabase(model->globalDatabase());
            factory.setTreeModel(model->treeModel());
        }
    }

    /**
     * @brief Application::Application
     */
    Application::Application(QObject *parent)
        : QObject(parent)
        , m_ApplicationModel(std::make_shared<models::ApplicationModel>())
        , m_MainWindow(std::make_unique<gui::MainWindow>(m_ApplicationModel))
    {
        G_CONNECT(m_ApplicationModel.get(), &models::ApplicationModel::currentProjectChanged,
                  &helpers::GeneratorID::instance(), &helpers::GeneratorID::onCurrentProjectChanged);

        // Set up factories
        setUpFactory(entity::EntityFactory::instance(), m_ApplicationModel, m_MainWindow->scene());
        setUpFactory(relationship::RelationFactory::instance(), m_ApplicationModel,
                     m_MainWindow->scene());

        qRegisterMetaTypeStreamOperators<application::settings::TstType>("application::settings::TstType");
    }

    /**
     * @brief Application::~Application
     */
    Application::~Application()
    {
    }

} // namespace application
