/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 06/01/2015.
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
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QVBoxLayout>
#include <QSplitter>
#include <QTreeView>
#include <QLineEdit>
#include <QGraphicsView>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QGraphicsScene>
#include <QUndoView>
#include <QUndoStack>
#include <QDockWidget>
#include <QPointer>
#include <QMimeData>
#include <QDebug>

#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/for_each.hpp>

#include <application/settings.h>

#include <models/ApplicationModel.h>
#include <models/ProjectTreeModel.h>

#include <project/Project.h>

#include <gui/graphics/Entity.h>
#include <gui/graphics/Scene.h>

#include <entity/Type.h>

#include <commands/CreateScope.h>
#include <commands/MakeProjectCurrent.h>
#include <commands/RemoveProject.h>

#include "about.h"
#include "newproject.h"
#include "addscope.h"
#include "scenefilter.h"
#include "constants.h"
#include "Elements.h"
#include "View.h"
#include "qthelpers.h"

using namespace boost;

namespace {
    const int treeViewIndent = 20;
    const QSize iconSize(20, 20);

    const double treeSizeFactor = 0.3;
    const double canvasSizeFactor = 0.7;
    const double consoleSizeFactor = 0.3;

    const QString titleTemplate = "%1Q-UML";
}

namespace gui {

    /**
     * @brief MainWindow::MainWindow
     * @param parent
     */
    MainWindow::MainWindow(const models::SharedApplicationModel &applicationModel)
        : ui(std::make_unique<Ui::MainWindow>())
        , m_MainScene(std::make_unique<graphics::Scene>())
        , m_ProjectTreeMenu(new QMenu(this))
        , m_ProjectTreeView(new QTreeView(this))
        , m_MainView(new View(applicationModel, this))
        , m_ConsoleOutput(new QTextEdit(this))
        , m_UndoView(new QUndoView(this))
        , m_Elements(new Elements(this))
        , m_AboutWidget(new About(this))
        , m_NewProjectDialog(new NewProjectDialog(this))
        , m_AddScope(new AddScope(this))
        , m_ApplicationModel(applicationModel)
        , m_CommandsStack(std::make_shared<QUndoStack>())
    {
        ui->setupUi(this);

        setUpWidgets();
        configure();
        makeConnections();

        m_MainScene->installEventFilter(new SceneFilter(m_ApplicationModel, m_MainScene.get(),
                                                        this, this));
        update();
    }

    /**
     * @brief MainWindow::~MainWindow
     */
    MainWindow::~MainWindow()
    {
    }

    /**
     * @brief MainWindow::scene
     * @return
     */
    QPointer<QGraphicsScene> MainWindow::scene() const
    {
        return m_MainScene.get();
    }

    /**
     * @brief MainWindow::update
     */
    void MainWindow::update()
    {
        makeTitle();
        updateWindowState();
        m_ProjectTreeView->update();
    }

    /**
     * @brief MainWindow::onExit
     */
    void MainWindow::onExit()
    {
        if (maybeExit())
            close();
    }

    /**
     * @brief MainWindow::onAbout
     */
    void MainWindow::onAbout()
    {
        m_AboutWidget->show();
    }

    /**
     * @brief MainWindow::onNewProject
     */
    void MainWindow::onNewProject()
    {
        m_NewProjectDialog->show();
    }

    /**
     * @brief MainWindow::onOpenProject
     */
    void MainWindow::onOpenProject()
    {
        QString caption(tr("Selet project file"));
        QString dir(QApplication::applicationDirPath()); // temporary
        QString filter(tr("Q-UML Project files (*.%1)").arg(PROJECT_FILE_EXTENTION));

        QString path = QFileDialog::getOpenFileName(this, caption, dir, filter);
        if (path.isEmpty())
            return ;

        // Deactivate current project
        commands::SharedCommand deactivateCmd;
        if (m_ApplicationModel->currentProject()) {
            deactivateCmd = std::make_shared<commands::MakeProjectCurrent>("", m_ApplicationModel,
                                                                           m_MainScene.get());
            deactivateCmd->redo();
        }

        // Load new one
        auto newProject = std::make_shared<project::Project>();
        newProject->load(path);

        if (newProject->hasErrors()) {
            QMessageBox::critical
                ( this
                , tr("Open project error%1").arg(newProject->lastErrors().size() > 1 ? "s" : "")
                , newProject->lastErrors().join("\n")
                , QMessageBox::Ok
                );

            // Fallback to previous project
            if (deactivateCmd)
                deactivateCmd->undo();
        } else {
            newProject->setCommandsStack(m_CommandsStack);
            if (m_ApplicationModel->addProject(newProject))
                m_ApplicationModel->setCurrentProject(newProject->name());
            else
                QMessageBox::information
                    ( this
                    , tr("Q-UML - Information")
                    , tr("Project \"%1\" already exists.").arg(newProject->name())
                    , QMessageBox::Ok
                    );
        }
    }

    /**
     * @brief MainWindow::onSaveProject
     */
    void MainWindow::onSaveProject()
    {
        if (auto currentPtoject = m_ApplicationModel->currentProject()) {
            if (!currentPtoject->isSaved())
                currentPtoject->save();
        } else {
            QMessageBox::information(this, tr("Information"), tr("Nothing to save."), QMessageBox::Ok);
        }
    }

    /**
     * @brief MainWindow::onCloseProject
     */
    void MainWindow::onCloseProject()
    {
        if (m_ApplicationModel && m_ApplicationModel->currentProject()) {
            m_ApplicationModel->removeProject(m_ApplicationModel->currentProject()->name());
        }
    }

    /**
     * @brief MainWindow::onCreateScope
     */
    void MainWindow::onCreateScope()
    {
        if (m_ApplicationModel->currentProject()) {

            m_AddScope->setProjectName(m_ApplicationModel->currentProject()->name());

            QStringList lst;
            for (auto &&scope : m_ApplicationModel->currentProject()->database()->scopes())
                lst << scope->name();
            m_AddScope->addExistingScopesNames(lst);

            if (m_AddScope->exec())  {
                QString scopeName = m_AddScope->scopeName();
                if (!scopeName.isEmpty())
                    if (auto stack = m_ApplicationModel->currentProject()->commandsStack())
                        stack->push(std::make_unique<commands::CreateScope>(scopeName, *m_ApplicationModel).release());
            }
        } else
            QMessageBox::information(this, tr("Information"), tr("No current project."), QMessageBox::Ok);
    }

    /**
     * @brief MainWindow::createNewProject
     * @param name
     * @param path
     */
    void MainWindow::createNewProject(const QString &name, const QString &path)
    {
        if (project::SharedProject currentProject = m_ApplicationModel->currentProject()) {
            if (!currentProject->isSaved()) {
                int result =
                    QMessageBox::question
                        ( this
                        , tr("Project is not saved")
                        , tr("Would you like to save a project %1?").arg(currentProject->name())
                        , QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
                        );

                switch (result) {
                    case QMessageBox::Yes:
                        currentProject->save();
                        break;

                    case QMessageBox::No:
                        break;

                    case QMessageBox::Cancel:
                        return ;

                    default: ;
                }
            }
        }

        auto newProject = m_ApplicationModel->makeProject(name, path);
        newProject->setCommandsStack(m_CommandsStack);
        commands::MakeProjectCurrent(newProject->name(), m_ApplicationModel, m_MainScene.get()).redo();
        newProject->save();
    }

    /**
     * @brief MainWindow::createScope
     * @param name
     */
    void MainWindow::createScope(const QString &name)
    {
        if (!m_ApplicationModel->makeScope(name)) {
            QMessageBox::information(this, tr("Information"), tr("Cannot create scope."), QMessageBox::Ok);
        }
    }

    /**
     * @brief MainWindow::makeMenu
     */
    void MainWindow::makeMenu()
    {
        m_ChangeProjectStatusAction = m_ProjectTreeMenu->addAction("&Make active");
        m_ChangeProjectStatusAction->setIcon(QIcon(":/icons/pic/icon_accept_edit_dialog.png"));
        G_CONNECT(m_ChangeProjectStatusAction, &QAction::triggered,
                  this, &MainWindow::changeProjectStatus);

        auto a = m_ProjectTreeMenu->addAction("&Remove");
        a->setIcon(QIcon(":/icons/pic/icon_component_delete.png"));
        G_CONNECT(a, &QAction::triggered, this, &MainWindow::onRemoveActivated);
    }

    void MainWindow::setUpWidgets()
    {
        // Canvas
        m_MainView->setScene(m_MainScene.get());
        ui->gridLayout->addWidget(m_MainView);

        // Project
        m_ProjectTreeView->setHeaderHidden(true);
        m_ProjectTreeView->setIndentation(treeViewIndent);
        m_ProjectTreeView->setIconSize(iconSize());
        m_ProjectTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
        m_ProjectTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        m_ProjectTreeView->setModel(m_ApplicationModel->treeModel().get());
        addDock(tr("Projects"), ui->actionProjectsDockWidget, Qt::LeftDockWidgetArea, m_ProjectTreeView);

        // Project context menu
        makeMenu();

        // Add drag-and-drop entities
        addDock(tr("Elements"), ui->actionElementsDockWidget, Qt::LeftDockWidgetArea, m_Elements);

        // Commands
        addDock(tr("Commands"), ui->actionCommandsDockWidget, Qt::LeftDockWidgetArea, m_UndoView,
                false /*visible*/);

        // Messages
        m_ConsoleOutput->setReadOnly(true);
        addDock(tr("Messages"), ui->actionMessagesDockWidget, Qt::BottomDockWidgetArea,
                m_ConsoleOutput, false /*visible*/);
    }

    /**
     * @brief MainWindow::configure
     */
    void MainWindow::configure()
    {
        // Add relation actions
        ui->actionAddAssociation->setData(int(relationship::AssociationRelation));
        ui->actionAddDependency->setData(int(relationship::DependencyRelation));
        ui->actionAddGeneralization->setData(int(relationship::GeneralizationRelation));
        ui->actionAddAggregation->setData(int(relationship::AggregationRelation));
        ui->actionAddComposition->setData(int(relationship::CompositionRelation));

        m_RelationActions << ui->actionAddAssociation << ui->actionAddDependency
                          << ui->actionAddGeneralization << ui->actionAddAggregation
                          << ui->actionAddComposition;
    }

    /**
     * @brief MainWindow::makeConnections
     */
    void MainWindow::makeConnections()
    {
        G_CONNECT(m_ProjectTreeView, &QTreeView::customContextMenuRequested,
                  this, &MainWindow::onProjectTreeMenu);
        G_CONNECT(m_NewProjectDialog, &NewProjectDialog::newProject,
                  this, &MainWindow::createNewProject);
        G_CONNECT(m_ApplicationModel.get(), &models::ApplicationModel::currentProjectChanged,
                  this, &MainWindow::onCurrentProjectChanged);
        G_CONNECT(m_ApplicationModel.get(), &models::ApplicationModel::currentProjectChanged,
                  m_MainScene.get(), &graphics::Scene::onProjectChanged);
        G_CONNECT(m_MainScene.get(), &graphics::Scene::relationCompleted,
                  this, &MainWindow::onRelationCompleted);

        m_UndoView->setStack(m_CommandsStack.get());
        G_CONNECT(m_CommandsStack.get(), &QUndoStack::canRedoChanged,
                  ui->actionRedo, &QAction::setEnabled);
        G_CONNECT(m_CommandsStack.get(), &QUndoStack::canUndoChanged,
                  ui->actionUndo, &QAction::setEnabled);
        G_CONNECT(ui->actionRedo, &QAction::triggered,
                  m_CommandsStack.get(), &QUndoStack::redo);
        G_CONNECT(ui->actionUndo, &QAction::triggered,
                  m_CommandsStack.get(), &QUndoStack::undo);

        for (auto &&a : m_RelationActions) {
            G_CONNECT(a, &QAction::toggled, this, &MainWindow::onRelationActionToggled);
            G_CONNECT(a, &QAction::toggled, m_MainScene.get(), &graphics::Scene::setShowRelationTrack);
        }
    }

    /**
     * @brief MainWindow::readSettings
     */
    void MainWindow::readSettings()
    {
        setGeometry(application::settings::mainWindowGeometry());
    }

    /**
     * @brief MainWindow::writeSettings
     */
    void MainWindow::writeSettings()
    {
        application::settings::writeMainWindowGeometry(geometry());
    }

    /**
     * @brief MainWindow::addDock
     * @param name
     * @param area
     * @param widget
     * @param visible
     */
    void MainWindow::addDock(const QString &name, QAction * action, Qt::DockWidgetArea area,
                             QWidget *widget, bool visible)
    {
        QDockWidget * wgt = new QDockWidget(name, this);
        wgt->setWidget(widget);
        addDockWidget(area, wgt);

        connect(action, &QAction::toggled, wgt, &QDockWidget::setVisible);
        connect(wgt, &QDockWidget::visibilityChanged, action, &QAction::setChecked);

        wgt->setVisible(visible);
    }

    /**
     * @brief MainWindow::makeTitle
     */
    void MainWindow::makeTitle()
    {
        auto pr = m_ApplicationModel->currentProject();
        QString projectName = pr ? pr->name().append(pr->isSaved() ? "" : "*") : "";
        setWindowTitle(titleTemplate.arg(!projectName.isEmpty() ? projectName.append(" - ") : ""));
    }

    /**
     * @brief MainWindow::onTreeViewMenu
     * @param p
     */
    void MainWindow::onProjectTreeMenu(const QPoint &p)
    {
        QModelIndex index = m_ProjectTreeView->indexAt(p);
        QVariant data = index.data(models::ProjectTreeModel::SharedData);
        if (index.isValid() && data.canConvert<project::SharedProject>())
        {
            bool isCurrentProject =
                data.value<project::SharedProject>() == m_ApplicationModel->currentProject();
            m_ChangeProjectStatusAction->setText(isCurrentProject ? tr("Deactivate") : tr("Activate"));
            m_ProjectTreeMenu->exec(m_ProjectTreeView->mapToGlobal(p));
        }
    }

    /**
     * @brief MainWindow::setCurrentProject
     */
    void MainWindow::changeProjectStatus()
    {
        QModelIndex index = m_ProjectTreeView->selectionModel()->currentIndex();
        QVariant data = index.data(models::ProjectTreeModel::SharedData);
        if (index.isValid() && data.canConvert<project::SharedProject>()) {
            auto name = index.data(models::ProjectTreeModel::ID).toString();
            if (auto project = m_ApplicationModel->project(name))
            {
                // Check if we need to unset project
                if (m_ApplicationModel->currentProject() == project)
                    name.clear();

                auto cmd = std::make_unique<commands::MakeProjectCurrent>(name, m_ApplicationModel,
                                                                          m_MainScene.get());
                m_CommandsStack->push(cmd.release());
            }
        }
    }

    /**
     * @brief MainWindow::onRemoveProject
     * @param projectName
     */
    void MainWindow::onRemoveActivated()
    {
        auto index = m_ProjectTreeView->indexAt(m_ProjectTreeView->mapFromGlobal(m_ProjectTreeMenu->pos()));
        if (index.isValid()) {
            QVariant data = index.data(models::ProjectTreeModel::SharedData);
            if (data.canConvert<project::SharedProject>()) {
                auto name = index.data(models::ProjectTreeModel::ID).toString();
                if (auto project = m_ApplicationModel->project(name)) {
                    auto cmd = std::make_unique<commands::RemoveProject>(
                                   project, m_ApplicationModel, m_MainScene.get());
                    m_CommandsStack->push(cmd.release());
                }
            }
        }
    }

    /**
     * @brief MainWindow::setWindowState
     */
    void MainWindow::updateWindowState()
    {
        bool state = !!m_ApplicationModel->currentProject();

        ui->actionCreateScope->setEnabled(state);
        ui->actionSaveProject->setEnabled(state && !m_ApplicationModel->currentProject()->isSaved());
        ui->actionCloseProject->setEnabled(state);

        ui->actionRedo->setEnabled(m_CommandsStack->canRedo());
        ui->actionUndo->setEnabled(m_CommandsStack->canUndo());

        m_Elements->setEnabled(state);
        m_ProjectTreeView->setEnabled(!m_ApplicationModel->projects().isEmpty());
        m_MainView->setEnabled(state);

        boost::range::for_each(m_RelationActions, [&](auto &&a){ a->setEnabled(state); });
    }

    /**
     * @brief MainWindow::onRelationActionToggled
     * @param state
     */
    void MainWindow::onRelationActionToggled(bool checked)
    {
        if (checked) {
            QAction * sender = G_ASSERT(qobject_cast<QAction*>(QObject::sender()));
            for (auto &&a : m_RelationActions)
                if (a != sender)
                    a->setChecked(false);
        }
    }

    /**
     * @brief MainWindow::onCurrentProjectChanged
     * @param previous
     * @param current
     */
    void MainWindow::onCurrentProjectChanged(const project::SharedProject &previous,
                                             const project::SharedProject &current)
    {
        if (previous)
        {
            G_DISCONNECT(previous.get(), &project::Project::saved, this, &MainWindow::update);
            G_DISCONNECT(previous.get(), &project::Project::modified, this, &MainWindow::update);
        }

        if (current) {
            G_CONNECT(current.get(), &project::Project::saved, this, &MainWindow::update);
            G_CONNECT(current.get(), &project::Project::modified, this, &MainWindow::update);
        }

        update();
    }

    /**
     * @brief MainWindow::onRelationMade
     */
    void MainWindow::onRelationCompleted()
    {
        range::for_each(m_RelationActions, [](auto &&a){ a->setChecked(false); });
    }

    /**
     * @brief MainWindow::maybeExit
     * @return
     */
    bool MainWindow::maybeExit()
    {
        bool needExit = true;

        if (auto pr = m_ApplicationModel->currentProject()) {
            if (!pr->isSaved()) {
                int result = QMessageBox::question(this,
                                                   tr("Confirmation"),
                                                   tr("Project \"%1\" is not saved. Save?").arg(pr->name()),
                                                   QMessageBox::Abort | QMessageBox::Yes | QMessageBox::No);
                switch (result) {
                    case QMessageBox::Yes:
                        pr->save();
                    case QMessageBox::No:
                        break;
                    case QMessageBox::Abort:
                        needExit = false;
                        break;
                    default: ;
                }
            }
        }

        return needExit;
    }

    /**
     * @brief MainWindow::closeEvent
     * @param ev
     */
    void MainWindow::closeEvent(QCloseEvent *ev)
    {
        if (maybeExit()) {
            ev->accept();
            writeSettings();
        }
        else
            ev->ignore();
    }

    /**
     * @brief MainWindow::showEvent
     * @param ev
     */
    void MainWindow::showEvent(QShowEvent *ev)
    {
        readSettings();
        QMainWindow::showEvent(ev);
    }

} // namespace gui
