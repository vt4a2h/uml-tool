/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 24/03/2015.
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
#include "createscope.h"

#include <models/ApplicationModel.h>

#include "constants.h"

namespace commands {

    /**
     * @brief CreateScope::CreateScope
     * @param name
     * @param model
     * @param parent
     */
    CreateScope::CreateScope(const QString &name, models::ApplicationModel &model,
                             QUndoCommand *parent)
        : BaseCommand(tr("Create scope \"%1\"").arg(name), parent)
        , m_ScopeName(name)
        , m_Model(model)
    {
    }

    /**
     * @brief CreateScope::redo
     */
    void CreateScope::redo()
    {
        if (m_Done) {
            if (m_NewScope)
                m_Model.addExistsScope(m_NewScope);
        } else {
            // TODO: name should be unique (check all scopes and, if needed, make unique name)
            m_NewScope = m_Model.makeScope(m_ScopeName);
            m_Done = true; // done first time
        }
    }

    /**
     * @brief CreateScope::undo
     */
    void CreateScope::undo()
    {
        if (m_NewScope)
            m_Model.removeScope(m_NewScope->id());
    }

} // namespace commands
