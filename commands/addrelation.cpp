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

#include <utility/helpfunctions.h>

#include "qthelpers.h"

namespace commands {

    /**
     * @brief AddRelation::AddRelation
     * @param from
     * @param to
     */
    AddRelation::AddRelation(relationship::RelationType relType, const entity::SharedType &from,
                             const entity::SharedType &to, const db::SharedProjectDatabase &database)
        : BaseCommand(tr("Add relation"))
        , m_Type(relType)
        , m_From(from)
        , m_To(to)
        , m_Db(database)
    {
        Q_ASSERT(m_Db.lock() && m_From && m_To && !m_Scene.isNull());
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

        if (m_Done) {
            if (auto d = G_ASSERT(database())) {
                Q_ASSERT(!m_GraphicRelation.isNull());
                m_Scene->addItem(G_ASSERT(m_GraphicRelation.data()));

                Q_ASSERT(!d->containsRelation(m_Relation->id()));
                d->addRelation(m_Relation);
            }
        } else {
            if (auto d = G_ASSERT(database())) {

                m_Relation = G_ASSERT(utility::makeRelation(m_Type));
                m_Relation->setHeadType(m_To);
                m_Relation->setTailType(m_From);
                d->addRelation(m_Relation);

                m_GraphicRelation = new graphics::Relation(m_Relation, m_From, m_To);
                m_Scene->addItem(m_GraphicRelation.data());

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

    /**
     * @brief AddRelation::relation
     * @return
     */
    relationship::SharedRelation AddRelation::relation() const
    {
        return m_Relation;
    }

} // namespace commands

