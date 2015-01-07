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
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include <QSplitter>
#include <QTreeView>
#include <QLineEdit>
#include <QGraphicsView>
#include <QTextEdit>
#include <qdebug.h>

namespace gui {

    /**
     * @brief MainWindow::MainWindow
     * @param parent
     */
    MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
    {
        ui->setupUi(this);
        makeElemnts();
    }

    /**
     * @brief MainWindow::~MainWindow
     */
    MainWindow::~MainWindow()
    {
        delete ui;
    }

    /**
     * @brief MainWindow::makeElemnts
     */
    void MainWindow::makeElemnts()
    {
        m_MainVerticalSplitter = new QSplitter(this);
        m_ProjectTreeView = new QTreeView(this);
        m_MainVerticalSplitter->addWidget(m_ProjectTreeView);

        m_CanvasConsoleSplitter = new QSplitter(this);
        m_CanvasConsoleSplitter->setOrientation(Qt::Vertical);
        m_MainView = new QGraphicsView(this);
        m_ConsoleOutput = new QTextEdit(this);
        m_CanvasConsoleSplitter->addWidget(m_MainView);
        m_CanvasConsoleSplitter->addWidget(m_ConsoleOutput);
        m_CanvasConsoleSplitter->setStretchFactor(0, 9);
        m_MainVerticalSplitter->addWidget(m_CanvasConsoleSplitter);

        m_MainVerticalSplitter->setStretchFactor(1, 9);

        m_MainLayout = std::make_unique<QHBoxLayout>();
        m_MainLayout->addWidget(m_MainVerticalSplitter);
        ui->centralwidget->setLayout(m_MainLayout.get());

        int treeSize(std::round(this->width() * 0.3));
        int canvasConsoleSize(std::round(this->width() * 0.7));
        m_MainVerticalSplitter->setSizes({treeSize, canvasConsoleSize});

        int canvasSize(std::round(this->height() * 0.7));
        int consoleSize(std::round(this->height() * 0.3));
        m_CanvasConsoleSplitter->setSizes({canvasSize, consoleSize});
    }

} // namespace gui
