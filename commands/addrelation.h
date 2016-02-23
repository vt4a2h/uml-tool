/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 16/01/2016.
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

#include <gui/graphics/graphics_types.h>

#include <project/project_types.hpp>

#include <relationship/relationship_types.hpp>

#include "basecommand.h"

namespace graphics {
    class Entity;
    class Relation;
}

namespace commands {

    /// Add relation command
    class AddRelation : public BaseCommand
    {
    public:
        AddRelation(const graphics::EntityPtr &from, const graphics::EntityPtr &to);

        ~AddRelation();

    public: // QUndoCommand overrides
        void redo() override;
        void undo() override;

    private:
        project::SharedProject pr() const;

        graphics::EntityPtr m_From;
        graphics::EntityPtr m_To;

        graphics::UniqueGraphicRelation m_GraphicRelation;
        relationship::SharedRelation m_Relation;

        project::WeakProject m_Project;
    };

} // namespace commands
