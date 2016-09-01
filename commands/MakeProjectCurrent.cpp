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
#include "MakeProjectCurrent.h"
#include <QStringBuilder>

namespace commands {

    /**
     * @brief MakeProjectCurrent::MakeProjectCurrent
     * @param projectName
     * @param model
     */
    MakeProjectCurrent::MakeProjectCurrent(const QString &projectName,
                                           const models::SharedApplicationModel &model)
        : BaseCommand("Activate project -- " % projectName)
        , m_AppModel(model)
        , m_ProjectName(projectName)
    {
    }

    /**
     * @brief MakeProjectCurrent::undo
     */
    void MakeProjectCurrent::undo()
    {
        sanityCheck();

        G_ASSERT(m_AppModel->setCurrentProject(m_PreviousProjectName));
    }

    /**
     * @brief MakeProjectCurrent::redo
     */
    void MakeProjectCurrent::redo()
    {
        if (!m_Done)
        {
            if (auto p = m_AppModel->currentProject())
                m_PreviousProjectName = p->name();

            m_Done = true;
        }

        G_ASSERT(m_AppModel->setCurrentProject(m_ProjectName));
    }

} // namespace commands
