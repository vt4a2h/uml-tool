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
#include "addcomponentscommands.h"

#include "removecomponentscommands.h"

namespace commands {

    /**
     * @brief AddMethod::AddMethod
     * @param model
     * @param method
     * @param pos
     * @param parent
     */
    AddMethod::AddMethod(const models::SharedClassComponentsModel &model, const entity::SharedMethod &method,
                         int pos, QUndoCommand *parent)
        : AddComponentBaseCommand<entity::SharedMethod>(tr("Add new method"), model, method, pos, parent)
    {}

    /**
     * @brief AddMethod::redo
     */
    void AddMethod::redo()
    {
        redoImpl([this](){ m_Model->addExistsMethod(m_Component, m_Pos); }, [this](){ return m_Model->addMethod(); });
    }

    /**
     * @brief AddMethod::undo
     */
    void AddMethod::undo()
    {
        RemoveMethod(m_Model, m_Component).redo();
    }

    /**
     * @brief AddField::AddField
     * @param model
     * @param field
     * @param pos
     * @param parent
     */
    AddField::AddField(const models::SharedClassComponentsModel &model, const entity::SharedField &field, int pos,
                       QUndoCommand *parent)
        : AddComponentBaseCommand<entity::SharedField>(tr("Add new field"), model, field, pos, parent)
    {}

    /**
     * @brief AddField::redo
     */
    void AddField::redo()
    {
        redoImpl([this](){ m_Model->addExistsField(m_Component, m_Pos); }, [this](){ return m_Model->addField(); });
    }

    /**
     * @brief AddField::undo
     */
    void AddField::undo()
    {
        RemoveField(m_Model, m_Component).redo();
    }

    /**
     * @brief AddElement::AddElement
     * @param model
     * @param element
     * @param pos
     * @param parent
     */
    AddElement::AddElement(const models::SharedClassComponentsModel &model, const entity::SharedEnumarator &element,
                           int pos, QUndoCommand *parent)
        : AddComponentBaseCommand<entity::SharedEnumarator>(tr("Add new element"), model, element, pos, parent)
    {}

    /**
     * @brief AddElement::redo
     */
    void AddElement::redo()
    {
        redoImpl([this](){ m_Model->addExistsElement(m_Component, m_Pos); }, [this](){ return m_Model->addElement(); } );
    }

    /**
     * @brief AddElement::undo
     */
    void AddElement::undo()
    {
        RemoveElement(m_Model, m_Component).redo();
    }

} // namespace commands

