/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 03/05/2015.
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

#include <QObject>
#include <QStack>

#include "types.h"

namespace commands {

    class BaseCommand;

    /// The class UndoStack
    class UndoStack : public QObject
    {
        Q_OBJECT

    public:
        Q_DISABLE_COPY(UndoStack)

        explicit UndoStack(QObject *parent = nullptr);
        ~UndoStack() override;

        void push(commands::UniqueCommand &&cmd);

        bool canUndo() const;
        bool canRedo() const;

        void beginMacro(const QString &text);
        void endMacro();

    public slots:
        void redo();
        void undo();

    signals:
        void needRepaint();
        void needUpdateModelView();

        void canRedoChanged(bool val);
        void canUndoChanged(bool val);

    private:
        void checkStates(const commands::SharedCommand &cmd);
        void setUndoableRedoable();
        void setCanUndo(bool newUndoState);
        void setCanRedo(bool newRedoState);

        using Stack = QStack<commands::SharedCommand>;

        bool m_CanUndo = false;
        bool m_CanRedo = false;

        Stack m_RedoStack;
        Stack m_UndoStack;
    };

} // namespace commands
