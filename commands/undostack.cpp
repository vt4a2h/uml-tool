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
#include "undostack.h"

#include <QUndoStack>

#include "basecommand.h"

namespace commands {

    /**
     * @brief UndoStack::UndoStack
     * @param parent
     */
    UndoStack::UndoStack(QObject *parent)
        : QObject(parent)
    {
    }

    /**
     * @brief UndoStack::~UndoStack
     */
    UndoStack::~UndoStack()
    {
    }

    /**
     * @brief UndoStack::push
     * @param cmd
     */
    void UndoStack::push(commands::UniqueCommand &&cmd)
    {
        m_RedoStack.push(std::forward<commands::UniqueCommand>(cmd));
        redo();
    }

    /**
     * @brief UndoStack::canUndo
     * @return
     */
    bool UndoStack::canUndo() const
    {
        return m_CanUndo;
    }

    /**
     * @brief UndoStack::canRedo
     * @return
     */
    bool UndoStack::canRedo() const
    {
        return m_CanRedo;
    }

    /**
     * @brief UndoStack::beginMacro
     * @param text
     */
    void UndoStack::beginMacro(const QString &text)
    {
        Q_UNUSED(text);
//        m_QStack->beginMacro(text);
    }

    /**
     * @brief UndoStack::endMacro
     */
    void UndoStack::endMacro()
    {
        //        m_QStack->endMacro();
    }

    /**
     * @brief UndoStack::redo
     */
    void UndoStack::redo()
    {
        m_UndoStack.push(m_RedoStack.pop());
        m_UndoStack.top()->redo();

        setUndoableRedoable();
        checkStates(m_UndoStack.top());
    }

    /**
     * @brief UndoStack::undo
     */
    void UndoStack::undo()
    {
        m_RedoStack.push(m_UndoStack.pop());
        m_RedoStack.top()->undo();

        setUndoableRedoable();
        checkStates(m_RedoStack.top());
    }

    /**
     * @brief UndoStack::checkStates
     */
    void UndoStack::checkStates(const SharedCommand &cmd)
    {
        Q_ASSERT(cmd);

        if (cmd->updateScene())
            emit needRepaint();

        if (cmd->updateModelView())
            emit needUpdateModelView();
    }

    void UndoStack::setUndoableRedoable()
    {
        setCanUndo(!m_UndoStack.isEmpty());
        setCanRedo(!m_RedoStack.isEmpty());
    }

    void UndoStack::setCanUndo(bool newUndoState)
    {
        if (newUndoState != m_CanUndo) {
            m_CanUndo = newUndoState;
            emit canUndoChanged(m_CanUndo);
        }
    }

    void UndoStack::setCanRedo(bool newRedoState)
    {
        if (newRedoState != m_CanRedo) {
            m_CanRedo = newRedoState;
            emit canRedoChanged(m_CanRedo);
        }
    }

} // namespace commands
