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
#pragma once

#include <functional>

#include "basecommand.h"

#include <models/componentsmodel.h>
#include <models/models_types.hpp>

namespace commands {

    /// The RemoveComponentBaseCommand class
    template<class Component>
    class RemoveComponentBaseCommand : public BaseCommand
    {
    public:
        RemoveComponentBaseCommand(const QString &name, const models::SharedClassComponentsModel &model,
                                   const Component &component, int pos = -1, QUndoCommand *parent = nullptr)
            : BaseCommand(name, parent)
            , m_Model(model)
            , m_Components(model ? model->components() : nullptr)
            , m_Component(component)
            , m_Pos(pos)
        {}

    protected:
        void redoImpl(const std::function<int()> &deleter)
        {
            entity::SharedComponents tmp;
            if (m_Model->components() != m_Components)
            {
                tmp = m_Model->components();
                m_Model->setComponents(m_Components);
            }

            m_Pos = deleter();

            if (tmp)
                m_Model->setComponents(tmp);
        }

        models::SharedClassComponentsModel m_Model;
        entity::SharedComponents m_Components;
        Component m_Component;
        int m_Pos;
    };

    /// The RemoveMethod class
    class RemoveMethod : public RemoveComponentBaseCommand<entity::SharedMethod>
    {
    public:
        RemoveMethod(const models::SharedClassComponentsModel &model, const entity::SharedMethod &method, int pos = -1,
                     QUndoCommand *parent = nullptr);

        void redo() override;
        void undo() override;
    };

    /// The RemoveField class
    class RemoveField : public RemoveComponentBaseCommand<entity::SharedField>
    {
    public:
        RemoveField(const models::SharedClassComponentsModel &model, const entity::SharedField &field, int pos = -1,
                     QUndoCommand *parent = nullptr);

        void redo() override;
        void undo() override;
    };

    /// The RemoveElement class
    class RemoveElement : public RemoveComponentBaseCommand<entity::SharedElement>
    {
    public:
        RemoveElement(const models::SharedClassComponentsModel &model, const entity::SharedElement &element, int pos = -1,
                    QUndoCommand *parent = nullptr);

        void redo() override;
        void undo() override;
    };

} // namespace commands

