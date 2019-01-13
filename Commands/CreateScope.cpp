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
#include "CreateScope.h"

#include <Models/ApplicationModel.h>

namespace Commands {

    /**
     * @brief CreateScope::CreateScope
     * @param name
     * @param model
     * @param parent
     */
    CreateScope::CreateScope(const QString &name, DB::SharedDatabase projectDB,
                             QUndoCommand *parent)
        : BaseCommand(tr("Create scope \"%1\"").arg(name), parent)
        , m_ScopeName(name)
        , m_ProjectDB(std::move(G_ASSERT(projectDB)))
    {
    }

    /**
     * @brief CreateScope::redo
     */
    void CreateScope::redoImpl()
    {
        if (m_Done) {
            if (m_NewScope)
                m_ProjectDB->addExistsScope(m_NewScope);
            else
                setObsolete(true);
        } else {
            m_NewScope = m_ProjectDB->addScope(m_ScopeName);
            m_Done = true; // done first time
        }
    }

    /**
     * @brief CreateScope::undo
     */
    void CreateScope::undoImpl()
    {
        if (m_NewScope)
            m_ProjectDB->removeScope(m_NewScope->id());
        else
            setObsolete(true);
    }

    /**
     * @brief CreateScope::scope
     * @return
     */
    Entity::SharedScope CreateScope::scope() const
    {
        return m_NewScope;
    }

    /**
     * @brief CreateScope::scopeName
     * @return
     */
    QString CreateScope::scopeName() const
    {
        return m_ScopeName;
    }

} // namespace Commands
