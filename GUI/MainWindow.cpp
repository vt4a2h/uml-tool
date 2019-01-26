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
#include <QTableView>
#include <QToolButton>
#include <QHeaderView>

#include <range/v3/algorithm/for_each.hpp>

#include <Application/Settings.h>

#include <Models/ApplicationModel.h>
#include <Models/ProjectTreeModel.h>
#include <Models/MessagesModel.h>

#include <Project/Project.h>

#include <GUI/graphics/Entity.h>
#include <GUI/graphics/Scene.h>

#include <Entity/Type.h>

#include <Commands/CreateScope.h>
#include <Commands/MakeProjectCurrent.h>
#include <Commands/RemoveProject.h>
#include <Commands/OpenProject.h>

#include <Project/ProjectDB.hpp>
#include <Project/ProjectFactory.hpp>

#include "About.h"
#include "NewProject.h"
#include "addscope.h"
#include "Constants.h"
#include "Elements.h"
#include "View.h"
#include "QtHelpers.h"
#include "HtmlDelegate.h"
#include "Preferences.h"
#include "EntityProperties.h"

namespace {
    const int treeViewIndent = 20;

    const QString titleTemplate = "%1Q-UML";

    const QSize toolBarButtonSize(16, 16);

    void clearRecentProjectsMenu(QMenu &menu, const QStringList &recentProjectsList)
    {
        // Clear menu
        for (auto && a : menu.actions())
            if (recentProjectsList.contains(a->data().toString()))
                menu.removeAction(a);

        // Reset settings
        App::Settings::saveRecentProjects({});
    }

    void addRecentProject(const QString &projectName)
    {
        QStringList recentProjectsList = App::Settings::recentProjects();
        recentProjectsList.removeOne(projectName);
        recentProjectsList.prepend(projectName);

        if (recentProjectsList.count() > App::Settings::recentProjectsMaxCount())
            recentProjectsList.removeLast();

        App::Settings::saveRecentProjects(recentProjectsList);
    }

    void removeRecentProject(const QString &projectName)
    {
        QStringList recentProjectsList = App::Settings::recentProjects();
        G_ASSERT(recentProjectsList.removeOne(projectName));
        App::Settings::saveRecentProjects(recentProjectsList);
    }

    void setActionState(QAction & action, bool state, const QString &tooltip)
    {
        action.setEnabled(state);
        action.setToolTip(tooltip);
    }

    class MessageButton : public QToolButton
    {
    public:
        MessageButton(QWidget * parent, Models::IMessenger const& messenger)
            : QToolButton(parent)
            , m_Messenger(messenger)
        {}

    protected:
        void paintEvent(QPaintEvent *ev) override
        {
            QToolButton::paintEvent(ev);

            QPainter painter;
            painter.begin(this);
            painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform |
                                   QPainter::TextAntialiasing | QPainter::HighQualityAntialiasing);

            QPixmap p(":/icons/pic/icon_message.png");
            auto rect = ev->rect();
            rect.setWidth(rect.width() - padding());
            rect.setHeight(rect.height() - padding());
            rect.setTopLeft({rect.topLeft().x() + padding(), rect.topLeft().y() + padding()});
            painter.drawPixmap(rect, p.scaled(rect.size(),Qt::KeepAspectRatio, Qt::SmoothTransformation));

            auto unreadMessagesCount = m_Messenger.unreadMessagesCount();
            if (unreadMessagesCount != 0)
            {
                painter.setPen(Qt::white);
                painter.drawText(rect, Qt::AlignCenter,
                                 unreadMessagesCount > 99 ? QString("99+")
                                                          : QString::number(unreadMessagesCount));
                painter.end();
            }

            ev->accept();
        }

    private: // Data
        Models::IMessenger const& m_Messenger;

    private: // Methods
        static int padding() { return 4; }
    };
}

namespace GUI {

    /**
     * @brief MainWindow::MainWindow
     * @param parent
     */
    MainWindow::MainWindow(const Models::SharedApplicationModel &applicationModel)
        : ui(std::make_unique<Ui::MainWindow>())
        , m_CommandsStack(std::make_shared<QUndoStack>())
        , m_MainScene(std::make_unique<Graphics::Scene>(m_CommandsStack))
        , m_ProjectTreeMenu(new QMenu(this))
        , m_ProjectTreeView(new QTreeView(this))
        , m_MainView(new View(applicationModel, m_CommandsStack, this))
        , m_UndoView(new QUndoView(this))
        , m_Elements(new Elements(this))
        , m_AboutWidget(new About(this))
        , m_NewProjectDialog(new NewProjectDialog(this))
        , m_AddScope(new AddScope(this))
        , m_Preferences(new Preferences(applicationModel, this))
        , m_MessagesView(new QTableView(this))
        , m_MessagesModel(std::make_shared<Models::MessagesModel>())
        , m_EntityProperties(new EntityProperties(this))
        , m_ApplicationModel(applicationModel)
    {
        ui->setupUi(this);

        setUpWidgets();
        configure();
        makeConnections();
        update();
    }

    /**
     * @brief MainWindow::~MainWindow
     */
    MainWindow::~MainWindow() = default;

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
     * @brief MainWindow::messenger
     * @return
     */
    Models::SharedMessenger MainWindow::messenger() const
    {
        return m_MessagesModel;
    }

    /**
     * @brief MainWindow::commandsStack
     * @return
     */
    Commands::SharedCommandStack MainWindow::commandsStack() const
    {
        return m_CommandsStack;
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
     * @brief MainWindow::onOpenProject
     */
    void MainWindow::openProject(const QString &path)
    {
        Commands::OpenProject cmd(tr("Open new project"), path, m_ApplicationModel, scene());

        G_CONNECT(&cmd, &Commands::OpenProject::recentProjectAdded,
                  this, &MainWindow::onRecentProjectAdded);
        G_CONNECT(&cmd, &Commands::OpenProject::projectErrors, [&](auto &&errors) {
            QMessageBox::critical(this, tr("Cannot open project"), errors);
        });

        cmd.redo();
    }

    /**
     * @brief MainWindow::rebuildRecentProjectMenu
     */
    void MainWindow::rebuildRecentProjectMenu()
    {
        Q_ASSERT(App::Settings::recentProjects().count() < App::Settings::recentProjectsMaxCount());

        m_RecentProjects->clear();

        for (auto && p : App::Settings::recentProjects()) {
            auto action = m_RecentProjects->addAction(p);
            action->setData(p);
            G_CONNECT(action, &QAction::triggered, [this, path = p](){ openProject(path); });
        }

        m_RecentProjects->addSeparator();
        auto clearRecentProjects = m_RecentProjects->addAction(GUI::MainWindow::tr("C&lear"));
        G_CONNECT(clearRecentProjects, &QAction::triggered,
                  [&] { clearRecentProjectsMenu(*m_RecentProjects, App::Settings::recentProjects()); });
    }

    /**
     * @brief MainWindow::configureMessagesPanel
     */
    void MainWindow::configureMessagesPanel()
    {
        m_MessagesView->setModel(m_MessagesModel.get());
        m_MessagesView->verticalHeader()->hide();
        m_MessagesView->horizontalHeader()->hide();
        m_MessagesView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        m_MessagesView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
        m_MessagesView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        m_MessagesView->setItemDelegateForColumn(0, new HtmlDelegate(this));

        m_MessagesDock = addDock(tr("Messages"), ui->actionMessagesDockWidget,
                                 Qt::BottomDockWidgetArea, m_MessagesView, false /*visible*/);
        G_CONNECT(m_MessagesDock, &QDockWidget::visibilityChanged,
                  [this](bool v){
                      if (v) {
                          m_MessagesModel->markAllMessagesRead();
                          m_MessagesButton->update();
                      }
                   });
        m_MessagesModel->setViewStatusFunction([this]{ return m_MessagesDock->isVisible(); });
    }

    /**
     * @brief MainWindow::configureStatusBar
     */
    void MainWindow::configureStatusBar()
    {
        m_MessagesButton = new MessageButton(this, *m_MessagesModel);
        ui->statusBar->addPermanentWidget(m_MessagesButton);
        m_MessagesButton->resize(toolBarButtonSize);
        m_MessagesButton->setFocusPolicy(Qt::NoFocus);

        auto action = [this](){ m_MessagesDock->setVisible(!m_MessagesDock->isVisible()); };
        connect(m_MessagesButton, &QToolButton::clicked, action);
    }

    void MainWindow::onOpenProject()
    {
        QString caption(tr("Selet project file"));
        QString filter(tr("Q-UML Project files (*.%1)").arg(PROJECT_FILE_EXTENTION));

        QString dirPath = App::Settings::lastOpenProjectDir();
        Q_ASSERT(!dirPath.isEmpty());

        QString path = QFileDialog::getOpenFileName(this, caption, dirPath, filter);
        if (!path.isEmpty()) {
            openProject(path);
            App::Settings::setLastOpenProjectDir(QFileInfo(path).dir().absolutePath());
        }
    }

    /**
     * @brief MainWindow::onSaveProject
     */
    void MainWindow::onSaveProject()
    {
        if (auto currentPtoject = m_ApplicationModel->currentProject()) {
            if (currentPtoject->isModified())
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
            m_ApplicationModel->projectsDb().removeProject(m_ApplicationModel->currentProject());
        }
    }

    /**
     * @brief MainWindow::onCreateScope
     */
    void MainWindow::onCreateScope()
    {
        if (auto p = m_ApplicationModel->currentProject()) {

            m_AddScope->setProjectName(p->name());

            QStringList lst;
            for (auto &&scope : p->database()->scopes())
                lst << scope->name();
            m_AddScope->addExistingScopesNames(lst);

            if (m_AddScope->exec())  {
                QString scopeName = m_AddScope->scopeName();
                if (!scopeName.isEmpty())
                    G_ASSERT(m_CommandsStack)->push(
                        Commands::make<Commands::CreateScope>(
                            scopeName, p->database()).release());
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
        if (Projects::SharedProject currentProject = m_ApplicationModel->currentProject()) {
            if (currentProject->isModified()) {
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

        auto newProject = Projects::ProjectFactory::instance().makeProject(name, path);
        Commands::MakeProjectCurrent(newProject->name(), m_ApplicationModel, m_MainScene.get()).redoImpl();
        newProject->save();

        addRecentProject(newProject->fullPath());
        rebuildRecentProjectMenu();
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

        // Entity properties
        addDock(tr("Properties"), ui->actionProperties, Qt::RightDockWidgetArea, m_EntityProperties,
                true /*visible*/);

        // Messages
        configureMessagesPanel();

        // Status bar
        configureStatusBar();

        // Recent projects
        m_RecentProjects = std::make_unique<QMenu>(tr("&Recent projects"));
        rebuildRecentProjectMenu();

        // Set action exit
        ui->menuFile->insertMenu(ui->actionExit, m_RecentProjects.get());
        ui->menuFile->insertSeparator(ui->actionExit);
    }

    /**
     * @brief MainWindow::configure
     */
    void MainWindow::configure()
    {
        // Add relation actions
        ui->actionAddAssociation->setData(int(Relationship::AssociationRelation));
        ui->actionAddDependency->setData(int(Relationship::DependencyRelation));
        ui->actionAddGeneralization->setData(int(Relationship::GeneralizationRelation));
        ui->actionAddAggregation->setData(int(Relationship::AggregationRelation));
        ui->actionAddComposition->setData(int(Relationship::CompositionRelation));

        m_RelationActions << ui->actionAddAssociation    << ui->actionAddDependency
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
        G_CONNECT(m_ApplicationModel.get(), &Models::ApplicationModel::currentProjectChanged,
                  this, &MainWindow::onCurrentProjectChanged);
        G_CONNECT(m_ApplicationModel.get(), &Models::ApplicationModel::currentProjectChanged,
                  m_MainScene.get(), &Graphics::Scene::onProjectChanged);
        G_CONNECT(m_ApplicationModel->globalDatabase().get(), &DB::Database::loaded,
                  this, &MainWindow::updateWindowState);
        G_CONNECT(m_MainScene.get(), &Graphics::Scene::relationCompleted,
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
            G_CONNECT(a, &QAction::toggled, m_MainScene.get(), &Graphics::Scene::setShowRelationTrack);
        }

        G_CONNECT(m_Preferences, &Preferences::preferencesChanged, this, &MainWindow::update);
        G_CONNECT(m_Preferences, &Preferences::globalDatabaseChanged,
                  this, &MainWindow::globalDatabaseChanged);

        G_CONNECT(ui->actionPreferences, &QAction::triggered, m_Preferences, &QWidget::show);
        G_CONNECT(ui->actionAbout, &QAction::triggered, m_AboutWidget, &QWidget::show);
        G_CONNECT(ui->actionNewProject, &QAction::triggered, m_NewProjectDialog, &QWidget::show);

        G_CONNECT(m_MainScene.get(), &Graphics::Scene::selectedItemsChanged,
                  m_EntityProperties, &EntityProperties::onSelectedElementsChanged);
        G_CONNECT(m_EntityProperties, &EntityProperties::sceneUpdateRequired,
                  m_MainScene.get(), qOverload<const QRectF &>(&QGraphicsScene::update));
    }

    /**
     * @brief MainWindow::readSettings
     */
    void MainWindow::readSettings()
    {
        setGeometry(App::Settings::mainWindowGeometry());
    }

    /**
     * @brief MainWindow::writeSettings
     */
    void MainWindow::writeSettings()
    {
        App::Settings::writeMainWindowGeometry(geometry());
    }

    /**
     * @brief MainWindow::addDock
     * @param name
     * @param area
     * @param widget
     * @param visible
     */
    QDockWidget * MainWindow::addDock(const QString &name, QAction * action, Qt::DockWidgetArea area,
                             QWidget *widget, bool visible)
    {
        QDockWidget * wgt = new QDockWidget(name, this);
        wgt->setWidget(widget);
        addDockWidget(area, wgt);

        connect(action, &QAction::toggled, wgt, &QDockWidget::setVisible);
        connect(wgt, &QDockWidget::visibilityChanged, action, &QAction::setChecked);

        wgt->setVisible(visible);
        return wgt;
    }

    /**
     * @brief MainWindow::makeTitle
     */
    void MainWindow::makeTitle()
    {
        auto pr = m_ApplicationModel->currentProject();
        QString projectName = pr ? pr->name().append(pr->isModified() ? "*" : "") : "";
        setWindowTitle(titleTemplate.arg(!projectName.isEmpty() ? projectName.append(" - ") : ""));
    }

    /**
     * @brief MainWindow::onTreeViewMenu
     * @param p
     */
    void MainWindow::onProjectTreeMenu(const QPoint &p)
    {
        QModelIndex index = m_ProjectTreeView->indexAt(p);
        QVariant data = index.data(Models::ProjectTreeModel::SharedData);
        if (index.isValid() && data.canConvert<Projects::SharedProject>())
        {
            bool isCurrentProject =
                data.value<Projects::SharedProject>() == m_ApplicationModel->currentProject();
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
        QVariant data = index.data(Models::ProjectTreeModel::SharedData);
        if (index.isValid() && data.canConvert<Projects::SharedProject>()) {
            auto name = index.data(Models::ProjectTreeModel::ID).toString();
            if (auto project = m_ApplicationModel->projectsDb().projectByName(name))
            {
                // Check if we need to unset project
                if (m_ApplicationModel->currentProject() == project)
                    name.clear();

                auto cmd = Commands::make<Commands::MakeProjectCurrent>(name, m_ApplicationModel,
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
            QVariant data = index.data(Models::ProjectTreeModel::SharedData);
            if (data.canConvert<Projects::SharedProject>()) {
                auto name = index.data(Models::ProjectTreeModel::ID).toString();
                if (auto project = m_ApplicationModel->projectsDb().projectByName(name)) {
                    auto cmd = Commands::make<Commands::RemoveProject>(
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
        ui->actionSaveProject->setEnabled(state && m_ApplicationModel->currentProject()->isModified());
        ui->actionCloseProject->setEnabled(state);

        ui->actionRedo->setEnabled(m_CommandsStack->canRedo());
        ui->actionUndo->setEnabled(m_CommandsStack->canUndo());

        m_Elements->setEnabled(state);
        m_ProjectTreeView->setEnabled(!m_ApplicationModel->projectsDb().isEmpty());
        m_MainView->setEnabled(state);

        ranges::for_each(m_RelationActions, [&](auto &&a){ a->setEnabled(state); });

        bool validDb = m_ApplicationModel->globalDatabase() &&
                       m_ApplicationModel->globalDatabase()->valid();
        setActionState(*ui->actionNewProject, validDb, validDb ? tr("Create new project")
                                                              : tr("Global database is not set"));
        setActionState(*ui->actionOpenProject, validDb, validDb ? tr("Open existing project")
                                                               : tr("Global database is not set"));
        m_RecentProjects->setEnabled(validDb);
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
    void MainWindow::onCurrentProjectChanged(const Projects::SharedProject &previous,
                                             const Projects::SharedProject &current)
    {
        if (previous)
            G_DISCONNECT(previous.get(), &Projects::Project::modifiedStatusUpdated,
                         this, &MainWindow::update);

        if (current)
            G_CONNECT(current.get(), &Projects::Project::modifiedStatusUpdated,
                      this, &MainWindow::update);

        update();
    }

    /**
     * @brief MainWindow::onRelationMade
     */
    void MainWindow::onRelationCompleted()
    {
        ranges::for_each(m_RelationActions, [](auto &&a){ a->setChecked(false); });
    }

    /**
     * @brief MainWindow::onRecentProjectRemoved
     * @param path
     */
    void MainWindow::onRecentProjectRemoved(const QString &path)
    {
        removeRecentProject(path);
        rebuildRecentProjectMenu();
    }

    /**
     * @brief MainWindow::onRecentProjectAdded
     */
    void MainWindow::onRecentProjectAdded(const QString &path)
    {
        addRecentProject(path);
        rebuildRecentProjectMenu();
    }

    /**
     * @brief MainWindow::maybeExit
     * @return
     */
    bool MainWindow::maybeExit()
    {
        bool needExit = true;

        if (auto pr = m_ApplicationModel->currentProject()) {
            if (pr->isModified()) {
                int result = QMessageBox::question(this,
                                                   tr("Confirmation"),
                                                   tr("Project \"%1\" is not saved. Save?").arg(pr->name()),
                                                   QMessageBox::Abort | QMessageBox::Yes | QMessageBox::No);
                switch (result) {
                    case QMessageBox::Yes:
                        pr->save();
                        [[fallthrough]];
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
