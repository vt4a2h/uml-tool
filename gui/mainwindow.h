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

#include <models/models_types.hpp>

class QSplitter;
class QHBoxLayout;
class QVBoxLayout;
class QTreeView;
class QGraphicsView;
class QTextEdit;
class QGraphicsScene;
class QUndoView;
class QUndoCommand;

namespace models {
    class ProjectTreeModel;
    class ApplicationModel;
}

/**
 * @brief namespace gui
 */
namespace gui {

    class About;
    class NewProject;
    class AddScope;
    class ChooseGlobalDatabaseDialog;
    class Elements;
    class View;

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

        explicit MainWindow(const models::SharedApplicationModel &applicationModel,
                            QWidget *parent = 0);
        ~MainWindow();

        void update();

//        bool eventFilter(QObject *obj, QEvent *ev) override;

    public slots:
        void onCreateScope();

    private slots:
        void onExit();
        void onAbout();
        void onNewProject();
        void onOpenProject();
        void onSaveProject();

        void onMakeRelation();

        void createNewProject(const QString &name, const QString &path);
        void createScope(const QString &name);
        void makeTitle();

        void onProjectTreeMenu(const QPoint &p);
        void setCurrentProjectViaMenu();

        void updateWindowState();
        void updateScene();
        void updateModelView();

        void setCurrentProject(const QString &id);

    private:
        bool maybeExit();
        void closeEvent(QCloseEvent *ev) override;
        void showEvent(QShowEvent *ev) override;

        void setUpWidgets();
        void makeConnections();

        void readSettings();
        void writeSettings();

        void configureAddActions();

        void addDock(const QString &name, QAction * action, Qt::DockWidgetArea area, QWidget * widget,
                     bool visible = true);

        QScopedPointer<Ui::MainWindow> ui;

        QMenu *m_ProjectTreeMenu;

        QTreeView      *m_ProjectTreeView;
        QGraphicsView  *m_MainView;
        QGraphicsScene *m_MainScene;
        QTextEdit      *m_ConsoleOutput;
        QUndoView      *m_UndoView;
        Elements       *m_Elements;

        About      *m_AboutWidget;
        NewProject *m_NewProject;
        AddScope   *m_AddScope;

        using CommandFunction = std::function<std::unique_ptr<QUndoCommand>(
                                    const models::SharedApplicationModel &, const QString &, QGraphicsScene &,
                                    const QPointF &, QUndoCommand *)>;
        using ActionMaker = QPair<QAction*, CommandFunction>;
        QList<ActionMaker> m_AddActions;

        models::SharedApplicationModel m_ApplicationModel;
    };

} // namespace gui
