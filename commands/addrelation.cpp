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
#include "addrelation.h"

#include <QGraphicsScene>

#include <gui/graphics/entity.h>
#include <gui/graphics/graphicsrelation.h>

#include <relationship/relation.h>

#include "qthelpers.h"

namespace commands {

    /**
     * @brief AddRelation::AddRelation
     * @param from
     * @param to
     */
    AddRelation::AddRelation(const graphics::EntityPtr &from,
                             const graphics::EntityPtr &to)
        : m_From(from)
        , m_To(to)
    {
        Q_ASSERT(m_From && m_To && m_From->scene() == m_To->scene());
    }

    /**
     * @brief AddRelation::redo
     */
    void AddRelation::redo()
    {
        Q_ASSERT(m_From && m_To);
        auto scene = G_ASSERT(m_From->scene());

        if (m_Done) {
            scene->addItem(G_ASSERT(m_Relation));
        } else {
            auto relationData = std::make_shared<relationship::Relation>(); // Just for test now
            m_Relation = new graphics::Relation(relationData, m_From, m_To);
            scene->addItem(m_Relation.data());

            m_Done = true;
        }

         m_CleaningRequired = false;
    }

    /**
     * @brief AddRelation::undo
     */
    void AddRelation::undo()
    {
        Q_ASSERT(m_From && m_To);
        auto scene = G_ASSERT(m_From->scene());

        scene->removeItem(m_Relation.data());
        m_CleaningRequired = true;
    }

    /**
     * @brief AddRelation::cleanup
     */
    void AddRelation::cleanup()
    {
        if (m_CleaningRequired && m_Relation)
            delete m_Relation.data();
    }

} // namespace commands

