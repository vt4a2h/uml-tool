/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 24/03/2015.
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

#include "BaseCommand.h"

#include <Entity/EntityTypes.hpp>
#include <Models/ModelsTypes.hpp>
#include <DB/DBTypes.hpp>

namespace Models {
    class ApplicationModel;
}

/// namespace Commands
namespace Commands {

    /// The CreateScope class
    class CreateScope : public BaseCommand
    {
    public:
        CreateScope(const QString &name, DB::SharedDatabase projectDB,
                    QUndoCommand *parent = nullptr);

        void redoImpl() override;
        void undoImpl() override;

        QString scopeName() const;
        Entity::SharedScope scope() const;

    private:
        QString m_ScopeName;
        Entity::SharedScope m_NewScope;
        DB::SharedDatabase m_ProjectDB;
    };

} // namespace Commands
