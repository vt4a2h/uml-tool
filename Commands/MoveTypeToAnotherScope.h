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
#pragma once

#include <Entity/entity_types.hpp>

#include <Models/ModelsTypes.hpp>

#include "BaseCommand.h"

namespace Commands {

    /// The MoveTypeToAnotherScope class
    class MoveTypeToAnotherScope : public BaseCommand
    {
    public:
        MoveTypeToAnotherScope(const Entity::SharedType &type, const Models::SharedApplicationModel &appModel,
                               const Entity::SharedScope &srcScope, const Entity::SharedScope &dstScope,
                               QUndoCommand *parent = nullptr);

        void redo() override;
        void undo() override;

    private:
        Entity::SharedType m_Type;

        Entity::SharedScope m_SrcScope;
        Entity::SharedScope m_DstScope;

        Models::SharedApplicationModel m_Model;
    };

} // namespace Commands
