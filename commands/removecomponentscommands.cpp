/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 11/06/2015.
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
#include "removecomponentscommands.h"

#include "addcomponentscommands.h"

namespace commands {

    /**
     * @brief RemoveMethod::RemoveMethod
     * @param model
     * @param method
     * @param pos
     * @param parent
     */
    RemoveMethod::RemoveMethod(const models::SharedClassComponentsModel &model, const entity::SharedMethod &method,
                               int pos, QUndoCommand *parent)
        : RemoveComponentBaseCommand<entity::SharedMethod>(tr("Remove method"), model, method, pos, parent)
    {}

    /**
     * @brief RemoveMethod::redo
     */
    void RemoveMethod::redo()
    {
        redoImpl([this](){ return m_Model->removeMethod(m_Component); });
    }

    /**
     * @brief RemoveMethod::undo
     */
    void RemoveMethod::undo()
    {
        AddMethod(m_Model, m_Component, m_Pos).redo();
    }

    /**
     * @brief RemoveField::RemoveField
     * @param model
     * @param field
     * @param pos
     * @param parent
     */
    RemoveField::RemoveField(const models::SharedClassComponentsModel &model, const entity::SharedField &field, int pos,
                             QUndoCommand *parent)
        : RemoveComponentBaseCommand<entity::SharedField>(tr("Remove field"), model, field, pos, parent)
    {}

    /**
     * @brief RemoveField::redo
     */
    void RemoveField::redo()
    {
        redoImpl([this](){ return m_Model->removeField(m_Component); });
    }

    /**
     * @brief RemoveField::undo
     */
    void RemoveField::undo()
    {
        AddField(m_Model, m_Component, m_Pos).redo();
    }

} // commands
