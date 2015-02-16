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
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

#include <models/applicationmodel.h>
#include <models/projecttreemodel.h>

#include <project/project.h>

#include "about.h"
#include "newproject.h"
#include "constants.cpp"

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
    MainWindow::MainWindow(const models::SharedApplicationModal &applicationModel, QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow)
        , m_ApplicationModel(applicationModel)
    {
        ui->setupUi(this);

        createMainWindowWidgets();
        createAdditionalWidgets();

        makeTitle();
    }

    /**
     * @brief MainWindow::~MainWindow
     */
    MainWindow::~MainWindow()
    {
        delete ui;
    }

    /**
     * @brief MainWindow::onExit
     */
    void MainWindow::onExit()
    {
        this->close();
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
        m_NewProject->show();
    }

    /**
     * @brief MainWindow::onOpenProject
     */
    void MainWindow::onOpenProject()
    {
        QString caption(tr("Selet project file"));
        QString dir(QApplication::applicationDirPath()); // temporary
        QString filter(tr("Q-UML Project files (*.%1)").arg(PROJECT_FILE_EXTENTION));

        QString file = QFileDialog::getOpenFileName(this, caption, dir, filter);
        Q_UNUSED(file)
    }

    /**
     * @brief MainWindow::onSaveProject
     */
    void MainWindow::onSaveProject()
    {

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

        auto newProject = m_ApplicationModel->makeProject( name, path );
        m_ApplicationModel->setCurrentProject(newProject->id());
        newProject->save();
        makeTitle(); // TODO: add signal to make it auto
    }

    /**
     * @brief MainWindow::makeElemnts
     */
    void MainWindow::createMainWindowWidgets()
    {
        m_MainVerticalSplitter = new QSplitter(this);
        m_ProjectTreeView = new QTreeView(this);
        m_ProjectTreeView->setHeaderHidden(true);
        m_ProjectTreeView->setIndentation(treeViewIndent);
        m_ProjectTreeView->setIconSize(iconSize());
        m_ProjectTreeView->setModel(m_ApplicationModel->treeModel().get());
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

        int treeSize(std::round(this->width() * treeSizeFactor));
        int canvasConsoleSize(std::round(this->width() * canvasSizeFactor));
        m_MainVerticalSplitter->setSizes({treeSize, canvasConsoleSize});

        int canvasSize(std::round(this->height() * canvasSizeFactor));
        int consoleSize(std::round(this->height() * consoleSizeFactor));
        m_CanvasConsoleSplitter->setSizes({canvasSize, consoleSize});
    }

    /**
     * @brief MainWindow::createAdditionalWidgets
     */
    void MainWindow::createAdditionalWidgets()
    {
        m_AboutWidget = new About(this);

        m_NewProject = new NewProject(this);
        connect(m_NewProject, &NewProject::newProject, this, &MainWindow::createNewProject);
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

} // namespace gui
