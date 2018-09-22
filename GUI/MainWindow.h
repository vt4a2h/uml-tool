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
#pragma once

#include <QMainWindow>

#include <Models/ModelsTypes.hpp>
#include <Project/ProjectTypes.hpp>
#include <Commands/CommandsTypes.h>
#include <GUI/graphics/GraphicsTypes.h>

QT_BEGIN_NAMESPACE
class QSplitter;
class QHBoxLayout;
class QVBoxLayout;
class QTreeView;
class QGraphicsView;
class QTextEdit;
class QGraphicsScene;
class QUndoView;
class QUndoCommand;
class QTableView;
class QToolButton;
QT_END_NAMESPACE

namespace Models {
    class ProjectTreeModel;
    class ApplicationModel;
}

namespace Entity { class ID; }
/**
 * @brief namespace gui
 */
namespace GUI {

    class About;
    class NewProjectDialog;
    class AddScope;
    class ChooseGlobalDatabaseDialog;
    class Elements;
    class View;
    class Preferences;
    class EntityProperties;

    namespace Ui {
        class MainWindow;
    }

    /**
     * @brief The MainWindow class
     */
    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        Q_DISABLE_COPY(MainWindow)

        explicit MainWindow(const Models::SharedApplicationModel &applicationModel);
        ~MainWindow() override;

        QPointer<QGraphicsScene> scene() const;

        Models::SharedMessenger messenger() const;

    public slots:
        void onCreateScope();
        void update();

    private slots:
        void onExit();
        void onOpenProject();
        void onSaveProject();
        void onCloseProject();

        void createNewProject(const QString &name, const QString &path);
        void createScope(const QString &name);
        void makeTitle();

        void onProjectTreeMenu(const QPoint &p);
        void changeProjectStatus();
        void onRemoveActivated();

        void updateWindowState();

        void onRelationActionToggled(bool checked);
        void onCurrentProjectChanged(const Projects::SharedProject &previous,
                                     const Projects::SharedProject &current);
        void onRelationCompleted();

        void onRecentProjectAdded(const QString &path);
        void onRecentProjectRemoved(const QString &path);

    signals:
        void globalDatabaseChanged(const QString &path, const QString &name);

    private:
        bool maybeExit();
        void closeEvent(QCloseEvent *ev) override;
        void showEvent(QShowEvent *ev) override;

        void setUpWidgets();
        void configure();
        void makeConnections();

        void readSettings();
        void writeSettings();

        void makeMenu();

        QDockWidget * addDock(const QString &name, QAction * action, Qt::DockWidgetArea area,
                              QWidget * widget, bool visible = true);

        void openProject(const QString &path);
        void rebuildRecentProjectMenu();

        void configureMessagesPanel();
        void configureStatusBar();

        std::unique_ptr<Ui::MainWindow> ui;

        Commands::SharedCommandStack m_CommandsStack;

        Graphics::UniqueScene m_MainScene;

        QMenu *m_ProjectTreeMenu;
        QAction *m_ChangeProjectStatusAction;

        QTreeView        *m_ProjectTreeView;
        View             *m_MainView;
        QUndoView        *m_UndoView;
        Elements         *m_Elements;

        About            *m_AboutWidget;
        NewProjectDialog *m_NewProjectDialog;
        AddScope         *m_AddScope;
        Preferences      *m_Preferences;

        QTableView                  *m_MessagesView;
        QDockWidget                 *m_MessagesDock;
        Models::SharedMessagesModel  m_MessagesModel;

        EntityProperties *m_EntityProperties;

        QPointer<QToolButton> m_MessagesButton;

        QList<QAction*> m_RelationActions;

        std::unique_ptr<QMenu> m_RecentProjects;
        QPointer<QAction> m_ClearRecentProjects;

        Models::SharedApplicationModel m_ApplicationModel;
    };

} // namespace gui
