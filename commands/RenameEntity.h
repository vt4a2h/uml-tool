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
#pragma once

#include <common/common_types.h>

#include "BaseCommand.h"

namespace Commands
{

    /// The RenameEntity class
    class RenameEntity : public BaseCommand
    {
    public:
        RenameEntity(const common::SharedBasicEntity &entity, const QString &newName,
                     QUndoCommand *parent = nullptr);

        void redo() override;
        void undo() override;

    protected:
        void sanityCheck() override;

    private:
        common::SharedBasicEntity m_Entity;
        QString m_NewName;
        QString m_OldName;
    };

} // namespace commands
