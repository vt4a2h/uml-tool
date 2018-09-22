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
#pragma once

#include <QObject>
#include <QPointer>

#include <Models/ModelsTypes.hpp>

class QGraphicsScene;
class QMenu;

namespace Graphics {
    class GraphisEntity;
}

namespace Models {
    class ApplicationModel;
}

namespace GUI {

    class MainWindow;

    // TODO: move functionality to the scene class (or eliminate)

    /// The SceneFilter class
    class SceneFilter : public QObject
    {
        Q_OBJECT

    public:
        explicit SceneFilter(const Models::SharedApplicationModel &model,
                             QGraphicsScene *scene,
                             MainWindow *parentForm,
                             QObject *parent = 0);
        ~SceneFilter();

    protected:
        bool eventFilter(QObject *o, QEvent *e) override;

    private:
        void makeMenu();

        QPointer<QGraphicsScene> m_Scene;
        QPointer<Graphics::GraphisEntity> m_CurrentEntity;

        std::unique_ptr<QMenu> m_Menu;

        Models::SharedApplicationModel m_ApplicationModel;
    };

} // namespace gui
