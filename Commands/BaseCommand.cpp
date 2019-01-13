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
#include "BaseCommand.h"

namespace Commands {

    /**
     * @brief BaseCommand::BaseCommand
     * @param parent
     */
    BaseCommand::BaseCommand(BaseCommand::QUndoCommand *parent)
        : BaseCommand(tr("No name"), parent)
    {
    }

    /**
     * @brief BaseCommand::BaseCommand
     * @param text
     * @param parent
     */
    BaseCommand::BaseCommand(const QString &text, BaseCommand::QUndoCommand *parent)
        :  QUndoCommand(text, parent)
    {
    }

    /**
     * @brief BaseCommand::~BaseCommand
     */
    BaseCommand::~BaseCommand()
    {
        cleanup();
    }

    /**
     * @brief BaseCommand::setProjectModified
     * @param modified
     * @return
     */
    void BaseCommand::setProjectModified(bool modified)
    {
        m_ProjectModified = modified;
    }

    /**
     * @brief BaseCommand::undo
     */
    void BaseCommand::undo()
    {
        undoImpl();

        emit changeProjectStatus(m_ProjectModified);
    }

    void BaseCommand::redo()
    {
        redoImpl();

        emit changeProjectStatus(modifiesProject());
    }

    /**
     * @brief BaseCommand::changesProjectStatus
     * @return
     */
    bool BaseCommand::modifiesProject() const noexcept
    {
        return true;
    }

    /**
     * @brief BaseCommand::undoImpl
     */
    void BaseCommand::undoImpl()
    {
        Q_ASSERT_X(false, Q_FUNC_INFO,
                   "Should never be invoked. Implement this function in your class, please.");
    }

    /**
     * @brief BaseCommand::redoImpl
     */
    void BaseCommand::redoImpl()
    {
        Q_ASSERT_X(false, Q_FUNC_INFO,
                   "Should never be invoked. Implement this function in your class, please.");
    }

} // namespace Commands
