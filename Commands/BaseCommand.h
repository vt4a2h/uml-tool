/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 28/03/2015.
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

#include <QUndoCommand>

#include "CommandFactory.hpp" // Required everywhere if commands are used

namespace Commands {

    /// The Base Command class
    class BaseCommand : public QObject, public QUndoCommand
    {
        Q_OBJECT

    public:
        explicit BaseCommand(QUndoCommand *parent = nullptr);
        explicit BaseCommand(const QString &text, QUndoCommand *parent = nullptr);

        ~BaseCommand() override;

        void setProjectModified(bool modified);

        /// Actual undo implementation. Doesn't track project state
        virtual void undoImpl();

        /// Actual redo implementation. Doesn't track project state
        virtual void redoImpl();

    public: // QUndoCommand overrides
        void undo() override;
        void redo() override;

    protected:
        /// Perform some cleanups in destructor
        virtual void cleanup() {}

        /// Perform checking objects state
        virtual void sanityCheck() {}

        /// Indicates whether current project should be marked as "modified" after performing this command
        virtual bool modifiesProject() const noexcept;

    signals:
        void changeProjectStatus(bool modified);

    protected: // Date
        bool m_CleaningRequired = false;
        bool m_Done = false; // Do first time -- false, redo -- true
        bool m_ProjectModified = false;
    };

} // namespace Commands
