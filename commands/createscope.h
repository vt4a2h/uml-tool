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

#include <QCoreApplication>
#include <QUndoCommand>

namespace models {
    class ApplicationModel;
}

/// Commands
namespace commands {

    /// The CreateScope class
    class CreateScope : public QUndoCommand
    {
        Q_DECLARE_TR_FUNCTIONS(CreateScope)

    public:
        CreateScope(const QString &name, models::ApplicationModel &model,
                    QUndoCommand *parent = nullptr);

        void redo() override;
        void undo() override;

    private:
        QString m_ScopeName;
        QString m_NewScopeID;
        models::ApplicationModel &m_Model;
    };

} // namespace commands