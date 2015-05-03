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
        , m_QStack(new QUndoStack())
    {
    }

    /**
     * @brief UndoStack::~UndoStack
     */
    UndoStack::~UndoStack()
    {
    }

    /**
     * @brief UndoStack::qstack
     * @return
     */
    QUndoStack *UndoStack::qstack()
    {
        return const_cast<QUndoStack *>(const_cast<const UndoStack *>(this)->qstack());
    }

    /**
     * @brief UndoStack::qstack
     * @return
     */
    const QUndoStack *UndoStack::qstack() const
    {
        return m_QStack.data();
    }

    /**
     * @brief UndoStack::push
     * @param cmd
     */
    void UndoStack::push(BaseCommand *cmd)
    {
        m_QStack->push(cmd);
    }

    /**
     * @brief UndoStack::canUndo
     * @return
     */
    bool UndoStack::canUndo() const
    {
        return m_QStack->canUndo();
    }

    /**
     * @brief UndoStack::canRedo
     * @return
     */
    bool UndoStack::canRedo() const
    {
        return m_QStack->canRedo();
    }

    /**
     * @brief UndoStack::beginMacro
     * @param text
     */
    void UndoStack::beginMacro(const QString &text)
    {
        m_QStack->beginMacro(text);
    }

    /**
     * @brief UndoStack::endMacro
     */
    void UndoStack::endMacro()
    {
        m_QStack->endMacro();
    }

} // namespace commands
