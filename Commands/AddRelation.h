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

#include <GUI/graphics/GraphicsTypes.h>

#include <DB/DBTypes.hpp>

#include <Entity/entity_types.hpp>

#include <Relationship/relationship_types.hpp>

#include "BaseCommand.h"
#include "enums.h"

class QGraphicsScene;

namespace Graphics {
    class GraphisEntity;
    class Relation;
}

namespace Commands {

    /// Add relation command
    class AddRelation : public BaseCommand
    {
    public:
        AddRelation(Relationship::RelationType relType, const Common::ID &tail,
                    const Common::ID &head);

        ~AddRelation();

        Relationship::SharedRelation relation() const;

    public: // QUndoCommand overrides
        void redo() override;
        void undo() override;

    private:
        Relationship::RelationType m_Type;

        Common::ID m_Tail;
        Common::ID m_Head;

        Graphics::GraphicRelationPtr m_GraphicRelation;
        Relationship::SharedRelation m_Relation;
    };

} // namespace Commands
