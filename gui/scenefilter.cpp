/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 23/05/2015.
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
#include "scenefilter.h"

#include <QMenu>
#include <QEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneContextMenuEvent>

#include <models/applicationmodel.h>

#include "graphics/entity.h"
#include "editentitydialog.h"
#include "mainwindow.h"

namespace gui {

    /**
     * @brief SceneFilter::SceneFilter
     * @param model
     * @param scene
     * @param parentForm
     * @param parent
     */
    SceneFilter::SceneFilter(const models::SharedApplicationModel &model, QGraphicsScene *scene,
                             MainWindow *parentForm, QObject *parent)
        : QObject(parent)
        , m_Scene(scene)
        , m_Menu(std::make_unique<QMenu>())
        , m_EditDialog(std::make_unique<EditEntityDialog>(parentForm))
        , m_ApplicationModel(model)
    {
        makeMenu();

        connect(m_EditDialog.get(), &EditEntityDialog::needNewScope, parentForm, &MainWindow::onCreateScope);
        connect(m_ApplicationModel.get(), &models::ApplicationModel::scopeAdded,
                m_EditDialog.get(), &EditEntityDialog::onScopeAdded);
    }

    /**
     * @brief SceneFilter::~SceneFilter
     */
    SceneFilter::~SceneFilter()
    {
    }

    /**
     * @brief SceneFilter::eventFilter
     * @param o
     * @param e
     * @return
     */
    bool SceneFilter::eventFilter(QObject *o, QEvent *e)
    {
        Q_ASSERT(m_Scene);
        Q_ASSERT(m_ApplicationModel);

        if (o == m_Scene) {
            if (e->type() == QEvent::GraphicsSceneContextMenu) {
                auto pos = QCursor::pos();
                auto view = m_Scene->views()[0];

                if (QGraphicsItem * item = m_Scene->itemAt(view->mapToScene(view->mapFromGlobal(pos)), QTransform())){
                    // NOTE: use identification based on data property
                    if (graphics::Entity * entity = dynamic_cast<graphics::Entity*>(item)) {
                        m_CurrentEntity = entity;

                        m_Menu->exec(pos);

                        e->accept();

                        return true;
                    }
                }
            }
        }

        e->ignore();

        return false;
    }

    /**
     * @brief SceneFilter::makeMenu
     */
    void SceneFilter::makeMenu()
    {
        auto actionEdit = m_Menu->addAction(tr("Edit"));
        connect(actionEdit, &QAction::triggered,
                [this]() {
                    m_EditDialog->setData(m_ApplicationModel, m_CurrentEntity->typeObject());
                    m_EditDialog->exec();
                });

        m_Menu->addSeparator();
        m_Menu->addAction(tr("Delete"));
    }

} // namespace gui
