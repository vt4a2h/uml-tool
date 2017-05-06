/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 04/05/2015.
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
#include "MoveTypeToAnotherScope.h"

#include <Entity/Type.h>
#include <Entity/Scope.h>

#include <Models/ApplicationModel.h>

namespace Commands {

    /**
     * @brief MoveTypeToOtherScope::MoveTypeToOtherScope
     * @param type
     * @param appModel
     * @param srcScope
     * @param dstScope
     * @param parent
     */
    MoveTypeToAnotherScope::MoveTypeToAnotherScope(const Entity::SharedType &type,
                                                   const Models::SharedApplicationModel &appModel,
                                                   const Entity::SharedScope &srcScope,
                                                   const Entity::SharedScope &dstScope,
                                                   QUndoCommand *parent)
        : BaseCommand(tr(R"(Move "%1" from "%2" to "%3".)").arg(type->name(), srcScope->name(), dstScope->name()),
                      parent)
        , m_Type(type)
        , m_SrcScope(srcScope)
        , m_DstScope(dstScope)
        , m_Model(appModel)
    {
    }

    /**
     * @brief MoveTypeToOtherScope::redo
     */
    void MoveTypeToAnotherScope::redo()
    {
        m_Model->removeType(m_Model->currentProject()->name(), m_SrcScope->id(), m_Type->id());
        m_Model->addExistsType(m_Model->currentProject()->name(), m_DstScope->id(), m_Type);
    }

    /**
     * @brief MoveTypeToOtherScope::undo
     */
    void MoveTypeToAnotherScope::undo()
    {
        MoveTypeToAnotherScope(m_Type, m_Model, m_DstScope, m_SrcScope).redo();
    }

} // namespace Commands
