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

#include <GUI/MainWindow.h>
#include <GUI/chooseglobaldatabasedialog.h>

#include <Models/ApplicationModel.h>
#include <Models/MessagesModel.h>

#include <Helpers/GeneratorID.h>

#include <Entity/Type.h>
#include <Entity/EntityFactory.h>

#include <Relationship/RelationFactory.h>

#include "Settings.h"
#include "QtHelpers.h"

namespace App {

    /**
     * @brief Application::run
     */
    bool Application::run()
    {
        m_MainWindow->show();
        auto messenger = m_MainWindow->messenger();

        auto db = G_ASSERT(m_ApplicationModel->globalDatabase());
        db->setPath(Settings::globalDbPath());
        db->setName(Settings::globalDbName());

        QStringList errors;
        db->load(errors);

        if (!errors.isEmpty()) {
            messenger->addMessage(Models::MessageType::Warning,
                                  tr("Problems with loading global database"),
                                  errors.join("\n"));
            db->clear();
            errors.clear();

            GUI::ChooseGlobalDatabaseDialog dlg(db->name(), db->path(), m_MainWindow.get());
            if (dlg.exec() == QDialog::Accepted) {
                return updateGlobalDBParameters(dlg.path(), dlg.name());
            } else {
                messenger->addMessage(Models::MessageType::Error,
                                      tr("Global database is not set"),
                                      tr("You cannot create a new project or open an existing one.\n"
                                         "Try specifying new database in Preferences, please."));
                return false;
            }
        }

        return true;
    }

    /**
     * @brief Application::updateGlobalDBParameters
     * @param path
     * @param name
     */
    bool Application::updateGlobalDBParameters(const QString &path, const QString &name)
    {
        auto db = G_ASSERT(m_ApplicationModel->globalDatabase());
        auto messenger = m_MainWindow->messenger();

        db->setPath(path);
        db->setName(name);

        if (QFile::exists(db->fullPath())) {
            QStringList errors;
            db->load(errors);

            if (!errors.isEmpty()) {
                messenger->addMessage(Models::MessageType::Error,
                                      tr("Database reading error"),
                                      errors.join("\n"));
                return false;
            }
        } else {
            // Create new database
            if (!db->save()) {
                messenger->addMessage(Models::MessageType::Error,
                                      tr("Database creating error"),
                                      tr("Cannot create new database: %1.").arg(db->fullPath()));
                return false;
            }
        }

        Settings::setGlobalDbPath(db->path());
        Settings::setGlobalDbName(db->name());

        messenger->addMessage(Models::MessageType::Information,
                              tr("Global database changed"),
                              tr("New application database has been specified."));

        return true;
    }

    namespace {

        template <class Factory>
        void setUpFactory(const Factory& f, const Models::SharedApplicationModel &model,
                          const QPointer<QGraphicsScene>& scene)
        {
            G_CONNECT(model.get(), &Models::ApplicationModel::currentProjectChanged,
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
    Application::Application(const Models::SharedApplicationModel &appModel,
                             GUI::UniqueMainWindow mainWindow)
        : m_ApplicationModel(appModel)
        , m_MainWindow(std::move(mainWindow))
    {
        G_CONNECT(m_ApplicationModel.get(), &Models::ApplicationModel::currentProjectChanged,
                  &Helpers::GeneratorID::instance(), &Helpers::GeneratorID::onCurrentProjectChanged);

        // Set up factories
        setUpFactory(Entity::EntityFactory::instance(), m_ApplicationModel, m_MainWindow->scene());
        setUpFactory(Relationship::RelationFactory::instance(), m_ApplicationModel,
                     m_MainWindow->scene());
    }

} // namespace application
