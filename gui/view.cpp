/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 28/12/2015.
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
#include "view.h"

#include <QDropEvent>
#include <QByteArray>
#include <QMimeData>
#include <QDebug>

#include <project/project.h>
#include <db/projectdatabase.h>
#include <commands/createentity.h>
#include <models/applicationmodel.h>

#include "elements.h"
#include "qthelpers.h"

namespace gui {

    /**
     * @brief View::View
     * @param parent
     */
    View::View(const models::SharedApplicationModel &model, QWidget *parent)
        : QGraphicsView(parent)
        , m_ApplicationModel(model)
    {
        setAcceptDrops(true);

        G_CONNECT(appModel().get(), &models::ApplicationModel::currentProjectChanged,
                  this, &View::onCurrentProjectChanged);
    }

    /**
     * @brief View::dropEvent
     * @param de
     */
    void View::dropEvent(QDropEvent *de)
    {
        if (de->mimeData()->hasFormat(Elements::mimeDataType())) {
            QByteArray itemData = de->mimeData()->data(Elements::mimeDataType());
            QDataStream in(&itemData, QIODevice::ReadOnly);

            uint tmpType = uint(SchemeElements::ElementsCount);
            in >> tmpType;
            Q_ASSERT(in.status() == QDataStream::Ok && tmpType != uint(SchemeElements::ElementsCount));
            addElement(SchemeElements(tmpType), de->pos());

            de->acceptProposedAction();
        }
    }

    /**
     * @brief View::dragEnterEvent
     * @param de
     */
    void View::dragEnterEvent(QDragEnterEvent *de)
    {
        if (de->mimeData()->hasFormat(Elements::mimeDataType()))
            de->acceptProposedAction();
        else
            de->ignore();
    }

    /**
     * @brief View::dragMoveEvent
     * @param de
     */
    void View::dragMoveEvent(QDragMoveEvent *de)
    {
        if (de->mimeData()->hasFormat(Elements::mimeDataType()))
            de->acceptProposedAction();
        else
            de->ignore();
    }

    /**
     * @brief View::dragLeaveEvent
     * @param de
     */
    void View::dragLeaveEvent(QDragLeaveEvent *de)
    {
        de->ignore();
    }

    /**
     * @brief View::onCurrentProjectChanged
     * @param p
     */
    void View::onCurrentProjectChanged(const project::SharedProject &p)
    {
        m_Project = p;
    }

    /**
     * @brief View::project
     * @return
     */
    project::SharedProject View::project() const
    {
        return m_Project.lock();
    }

    /**
     * @brief View::appModel
     * @return
     */
    models::SharedApplicationModel View::appModel() const
    {
       return m_ApplicationModel.lock();
    }

    template <class ... Types>
    std::unique_ptr<QUndoCommand> makeCmd(SchemeElements type, Types &&... args)
    {
        switch (type) {
            case SchemeElements::Alias:
                return std::make_unique<commands::MakeAlias>(std::forward<Types >(args)...);

            case SchemeElements::Class:
                return std::make_unique<commands::MakeClass>(std::forward<Types >(args)...);

            case SchemeElements::Enum:
                    return std::make_unique<commands::MakeEnum>(std::forward<Types >(args)...);

            case SchemeElements::TemplateClass:
                return std::make_unique<commands::MakeTemplate>(std::forward<Types >(args)...);

            case SchemeElements::Union:
                return std::make_unique<commands::MakeUnion>(std::forward<Types >(args)...);

            default:
                return nullptr;
        }
    }

    /**
     * @brief View::addElement
     * @param type
     * @param eventPos
     */
    void View::addElement(SchemeElements type, const QPoint &eventPos)
    {
        if (auto pr = project()) {
            auto projectDb = G_ASSERT(pr->database());
            if (projectDb->anyScopes()) {
                auto scope = projectDb->defaultScope();
                if (!scope)
                    scope = G_ASSERT(projectDb->scopes().first());

                if (auto stack = pr->commandsStack()) {
                    auto pos = mapToScene(eventPos);
                    if (auto cmd = makeCmd(type, appModel(), scope->id(), *scene(), pos, nullptr))
                        stack->push(cmd.release());
                }
            }
        } else {
            qWarning() << Q_FUNC_INFO << ": there is no project.";
        }
    }

} // namespace gui

