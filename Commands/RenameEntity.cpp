/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 22/04/2015.
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
#include "RenameEntity.h"

#include <common/BasicElement.h>

#include "QtHelpers.h"

namespace Commands
{

    /**
     * @brief RenameEntity::RenameEntity
     * @param entity
     * @param newName
     * @param parent
     */
    RenameEntity::RenameEntity(const common::SharedBasicEntity &entity, const QString &newName,
                               QUndoCommand *parent)
        : BaseCommand(tr(R"(Change name from "%1" to "%2".)").arg(G_ASSERT(entity)->name(), newName), parent)
        , m_Entity(entity)
        , m_NewName(newName)
        , m_OldName(entity->name())
    {
    }

    /**
     * @brief RenameEntity::redo
     */
    void RenameEntity::redo()
    {
        sanityCheck();

        m_OldName = m_Entity->name();
        m_Entity->setName(m_NewName);
    }

    /**
     * @brief RenameEntity::undo
     */
    void RenameEntity::undo()
    {
        m_Entity->setName(m_OldName);
    }

    /**
     * @brief RenameEntity::sanityCheck
     */
    void RenameEntity::sanityCheck()
    {
        Q_ASSERT(m_Entity);
        Q_ASSERT(!m_NewName.isEmpty());
    }

} // namespace commands

