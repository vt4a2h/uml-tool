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

#include "types.h"

class QSplitter;
class QHBoxLayout;
class QVBoxLayout;
class QTreeView;
class QGraphicsView;
class QTextEdit;
class QGraphicsScene;
class QUndoView;

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

        explicit MainWindow(const models::SharedApplicationModal &applicationModel,
                            QWidget *parent = 0);
        ~MainWindow();

        void update();

        bool eventFilter(QObject *obj, QEvent *ev) override;

    private slots:
        void onExit();
        void onAbout();
        void onNewProject();
        void onOpenProject();
        void onSaveProject();

        void onCreateScope();
        void onAddAlias();
        void onAddUnion();
        void onAddStruct();
        void onAddClass();
        void onAddTemplate();
        void onMakeRelation();

        void createNewProject(const QString &name, const QString &path);
        void createScope(const QString &name);
        void makeTitle();

        void onProjectTreeMenu(const QPoint &p);
        void setCurrentProjectViaMenu();

        void updateWindowState();

        void setCurrentProject(const QString &id);

    private:
        bool maybeExit();
        void closeEvent(QCloseEvent *ev);

        void setUpWidgets();
        void makeConnections();

        QScopedPointer<Ui::MainWindow> ui;

        QSplitter *m_MainVerticalSplitter;
        QSplitter *m_TreeSplitter;
        QSplitter *m_CanvasConsoleSplitter;

        QMenu *m_ProjectTreeMenu;

        std::unique_ptr<QHBoxLayout> m_MainLayout;

        QTreeView *m_ProjectTreeView;
        QGraphicsView *m_MainView;
        QGraphicsScene *m_MainScene;
        QTextEdit *m_ConsoleOutput;
        QUndoView *m_UndoView;

        About *m_AboutWidget;
        NewProject *m_NewProject;
        AddScope *m_AddScope;

        models::SharedApplicationModal m_ApplicationModel;
    };

} // namespace gui
