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
#include "RemoveComponentsCommands.h"

#include "AddComponentsCommands.h"

namespace Commands {

    /**
     * @brief RemoveMethod::RemoveMethod
     * @param model
     * @param method
     * @param pos
     * @param parent
     */
    RemoveMethod::RemoveMethod(const Models::SharedClassComponentsModel &model, const Entity::SharedMethod &method,
                               int pos, QUndoCommand *parent)
        : RemoveComponentBaseCommand<Entity::SharedMethod>(tr("Remove method"), model, method, pos, parent)
    {}

    /**
     * @brief RemoveMethod::redo
     */
    void RemoveMethod::redoImpl()
    {
        redoImplComponent([this](){ return m_Model->removeMethod(m_Component); });
    }

    /**
     * @brief RemoveMethod::undo
     */
    void RemoveMethod::undoImpl()
    {
        AddMethod(m_Model, m_Component, m_Pos).redoImpl();
    }

    /**
     * @brief RemoveField::RemoveField
     * @param model
     * @param field
     * @param pos
     * @param parent
     */
    RemoveField::RemoveField(const Models::SharedClassComponentsModel &model, const Entity::SharedField &field, int pos,
                             QUndoCommand *parent)
        : RemoveComponentBaseCommand<Entity::SharedField>(tr("Remove field"), model, field, pos, parent)
    {}

    /**
     * @brief RemoveField::redo
     */
    void RemoveField::redoImpl()
    {
        redoImplComponent([this](){ return m_Model->removeField(m_Component); });
    }

    /**
     * @brief RemoveField::undo
     */
    void RemoveField::undoImpl()
    {
        AddField(m_Model, m_Component, m_Pos).redoImpl();
    }

    /**
     * @brief RemoveElement::RemoveElement
     * @param model
     * @param element
     * @param pos
     * @param parent
     */
    RemoveElement::RemoveElement(const Models::SharedClassComponentsModel &model, const Entity::SharedEnumarator &element,
                                 int pos, QUndoCommand *parent)
        : RemoveComponentBaseCommand<Entity::SharedEnumarator>(tr("Remove element"), model, element, pos, parent)
    {}

    /**
     * @brief RemoveElement::redo
     */
    void RemoveElement::redoImpl()
    {
        redoImplComponent([this](){ return m_Model->removeElement(m_Component); });
    }

    /**
     * @brief RemoveElement::undo
     */
    void RemoveElement::undoImpl()
    {
        AddElement(m_Model, m_Component, m_Pos).redoImpl();
    }

} // Commands
