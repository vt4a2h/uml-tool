/*****************************************************************************
**
** Copyright (C) 2018 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 03/10/2018.
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
#include "MementoCmd.hpp"

#include <Common/IOriginator.hpp>
#include <Common/Memento.hpp>

namespace Commands {

    /**
     * @brief Memento::Memento
     * @param originator
     */
    Memento::Memento(Common::SharedOriginator originator)
        : m_Originator(std::move(originator))
    {
    }

    /**
     * @brief Memento::undoImpl
     */
    void Memento::undoImpl()
    {
        if (G_ASSERT(m_PrevState && m_Originator))
            if (auto errors = m_Originator->importState(*m_PrevState))
                for (auto &&e: errors.value())
                    qDebug() << e;
    }

    /**
     * @brief Memento::redoImpl
     */
    void Memento::redoImpl()
    {
        if (!m_Done) {
            if (G_ASSERT(m_Originator))
                m_PrevState = m_Originator->exportState();
        }
    }

} // namespace Commands
