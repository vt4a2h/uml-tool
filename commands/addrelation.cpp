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

#include <db/projectdatabase.h>

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
    AddRelation::AddRelation(const db::SharedProjectDatabase &database, const graphics::EntityPtr &from,
                             const graphics::EntityPtr &to)
        : m_From(from)
        , m_To(to)
        , m_Db(database)
    {
        Q_ASSERT(m_Db.lock() && m_From && m_To && m_From->scene() == m_To->scene());
    }

    /**
     * @brief AddRelation::~AddRelation
     */
    AddRelation::~AddRelation()
    {
        if (!m_GraphicRelation.isNull()) {
            removeRelationFromScene();
            delete m_GraphicRelation.data();
        }
    }

    /**
     * @brief AddRelation::redo
     */
    void AddRelation::redo()
    {
        Q_ASSERT(m_From && m_To);
        auto scene = G_ASSERT(m_From->scene());

        if (m_Done) {
            if (auto d = G_ASSERT(database())) {
                Q_ASSERT(!m_GraphicRelation.isNull());
                scene->addItem(G_ASSERT(m_GraphicRelation.data()));

                Q_ASSERT(!d->containsRelation(m_Relation->id()));
                d->addRelation(m_Relation);
            }
        } else {
            if (auto d = G_ASSERT(database())) {

                // TODO: specify relation type
                m_Relation = std::make_shared<relationship::Relation>();
                d->addRelation(m_Relation);

                m_GraphicRelation = new graphics::Relation(m_Relation, m_From, m_To);
                scene->addItem(m_GraphicRelation.data());

                m_Done = true;
            }
        }
    }

    /**
     * @brief AddRelation::undo
     */
    void AddRelation::undo()
    {
        removeRelationFromScene();
        if (auto d = G_ASSERT(database()))
            d->removeRelation(m_Relation->id());
    }

    /**
     * @brief AddRelation::pr
     * @return
     */
    db::SharedProjectDatabase AddRelation::database() const
    {
        return m_Db.lock();
    }

    void AddRelation::removeRelationFromScene()
    {
        if (m_From && m_To) {
            auto scene = G_ASSERT(m_From->scene());

            if (!m_GraphicRelation.isNull())
                scene->removeItem(m_GraphicRelation.data());
        }
    }

} // namespace commands

