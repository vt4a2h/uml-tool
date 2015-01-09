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
#include <memory>

class QSplitter;
class QHBoxLayout;
class QVBoxLayout;
class QTreeView;
class QGraphicsView;
class QTextEdit;

/**
 * @brief namespace gui
 */
namespace gui {

    class About;
    class NewProject;

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
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void onExit();
        void onAbout();
        void onNewProject();
        void onOpenProject();
        void onSaveProject();

    private:
        void createMainWindowWidgets();
        void createAdditionalWidgets();

        Ui::MainWindow *ui;

        QSplitter *m_MainVerticalSplitter;
        QSplitter *m_CanvasConsoleSplitter;

        std::unique_ptr<QHBoxLayout> m_MainLayout;

        QTreeView *m_ProjectTreeView;
        QGraphicsView *m_MainView;
        QTextEdit *m_ConsoleOutput;

        About *m_AboutWidget;
        NewProject *m_NewProject;
    };

} // namespace gui
