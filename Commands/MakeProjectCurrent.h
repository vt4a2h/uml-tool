/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 31/08/2016.
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

#include <Models/ApplicationModel.h>
#include <GUI/graphics/GraphicsTypes.h>

#include "BaseCommand.h"

QT_BEGIN_NAMESPACE
class QGraphicsScene;
QT_END_NAMESPACE

namespace Commands {

    class MakeProjectCurrent : public BaseCommand
    {
    public:
        MakeProjectCurrent(const QString &projectName, const Models::SharedApplicationModel &model,
                           const Graphics::ScenePtr &scene);

    public: // QUndoCommand overrides
        void undoImpl() override;
        void redoImpl() override;

        QString currentProjectName() const;
        QString previousProjectName() const;

    public: // BaseCommand overrides
        bool modifiesProject() const noexcept override;

    private:
        void sanityCheck() override;

        Models::SharedApplicationModel m_AppModel;

        QString m_CurrentProjectName;
        QString m_PreviousProjectName;

        Graphics::GraphicItems m_CurrentGraphicItems;
        Graphics::GraphicItems m_PreviousGraphicItems;

        Graphics::ScenePtr m_Scene;
    };

} // namespace Commands
