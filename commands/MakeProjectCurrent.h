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

#include <models/ApplicationModel.h>

#include "BaseCommand.h"

namespace commands {

    class MakeProjectCurrent : public BaseCommand
    {
    public:
        MakeProjectCurrent(const QString &projectName, const models::SharedApplicationModel &model);

    public: // QUndoCommand overridies
        void undo() override;
        void redo() override;

    private:
        models::SharedApplicationModel m_AppModel;
        QString m_ProjectName;
        QString m_PreviousProjectName;
    };

} // namespace commands
