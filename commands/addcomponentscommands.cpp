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
        : BaseCommand(tr("Add new method"), parent)
        , m_Model(model)
        , m_Components(model ? model->components() : nullptr)
        , m_Method(method)
        , m_Pos(pos)
    {}

    /**
     * @brief AddMethod::redo
     */
    void AddMethod::redo()
    {
        if (m_Done) {
            Q_ASSERT(m_Method);

            entity::SharedComponents tmp;

            if (m_Model->components() != m_Components)
            {
                tmp = m_Model->components();
                m_Model->setComponents(m_Components);
            }

            m_Model->addExistsMethod(m_Method, m_Pos);

            if (tmp)
                m_Model->setComponents(tmp);
        } else {
            if (m_Method)
                m_Model->addExistsMethod(m_Method, m_Pos);
            else
                m_Method = m_Model->addMethod();
        }
    }

    /**
     * @brief AddMethod::undo
     */
    void AddMethod::undo()
    {
        RemoveMethod(m_Model, m_Method).redo();
    }

} // namespace commands

