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
#pragma once

#include <QGraphicsView>

#include <Commands/CommandsTypes.h>

#include <Models/ModelsTypes.hpp>

#include <Project/ProjectTypes.hpp>

namespace Models { class ApplicationModel; }
namespace Entity { enum class KindOfType : int; }

namespace GUI {

    /// The class View
    class View : public QGraphicsView
    {
        Q_OBJECT

    public:
        View(const Models::SharedApplicationModel &model, Commands::SharedCommandStack cs,
             QWidget *parent = nullptr);

    public: // QGraphicsView overrides
        void dropEvent(QDropEvent *de) override;
        void dragEnterEvent(QDragEnterEvent *de) override;
        void dragMoveEvent(QDragMoveEvent *de) override;
        void dragLeaveEvent(QDragLeaveEvent *de) override;

    public slots:
        void onCurrentProjectChanged(const Projects::SharedProject &previous,
                                     const Projects::SharedProject &current);

    private:
        Projects::SharedProject project() const;
        Models::SharedApplicationModel appModel() const;
        void addElement(Entity::KindOfType kindOfType, const QPoint &eventPos);

        Projects::WeakProject m_Project;
        Models::WeakApplicationModel m_ApplicationModel;
        Commands::SharedCommandStack m_CommandStack;
    };

} // namespace gui
