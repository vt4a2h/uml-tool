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
#include "AddRelation.h"

#include <QGraphicsScene>

#include <DB/ProjectDatabase.h>

#include <GUI/graphics/Entity.h>
#include <GUI/graphics/GraphicsRelation.h>

#include <Relationship/Relation.h>
#include <Relationship/RelationFactory.h>

#include <Project/Project.h>

#include <Utility/helpfunctions.h>

#include "QtHelpers.h"

namespace Commands {

    /**
     * @brief AddRelation::AddRelation
     * @param from
     * @param to
     */
    AddRelation::AddRelation(Relationship::RelationType relType, const Common::ID &tail,
                             const Common::ID &head)
        : BaseCommand(tr("Add relation"))
        , m_Type(relType)
        , m_Tail(tail)
        , m_Head(head)
    {
        Q_ASSERT(m_Tail.isValid() && m_Head.isValid());
    }

    /**
     * @brief AddRelation::~AddRelation
     */
    AddRelation::~AddRelation()
    {
        if (!m_GraphicRelation.isNull()) {
            const auto & factory = Relationship::RelationFactory::instance();
            if (auto &&scene = G_ASSERT(factory.scene())) {
                if (!scene->items().contains(m_GraphicRelation.data()))
                    delete m_GraphicRelation.data();
            }
        }
    }

    /**
     * @brief AddRelation::redo
     */
    void AddRelation::redo()
    {
        Q_ASSERT(m_Tail.isValid() && m_Head.isValid());

        const auto & factory = Relationship::RelationFactory::instance();

        if (m_Done) {
            if (auto pr = factory.project()) {
                if (auto projectDb = pr->database()) {
                    Q_ASSERT(!projectDb->containsRelation(m_Relation->id()));
                    projectDb->addRelation(m_Relation);
                }

                if (auto scene = factory.scene()) {
                    Q_ASSERT(!m_GraphicRelation.isNull());
                    scene->addItem(G_ASSERT(m_GraphicRelation.data()));
                }

                // Add also to the tree model (when it'll be supported)
            }
        } else {
            m_Relation = G_ASSERT(factory.make(m_Type, m_Tail, m_Head));

            if (auto pr = factory.project())
                if (auto projectDb = pr->database())
                    m_GraphicRelation = G_ASSERT(projectDb->graphicRelation(m_Relation->id()));
        }
    }

    /**
     * @brief AddRelation::undo
     */
    void AddRelation::undo()
    {
        const auto & factory = Relationship::RelationFactory::instance();
        if (auto pr = factory.project()) {
            if (auto projectDb = pr->database())
                projectDb->removeRelation(m_Relation->id());

            if (auto scene = factory.scene())
                scene->removeItem(m_GraphicRelation.data());

            // TODO: remove also from tree model when it'll be supported
        }
    }

    /**
     * @brief AddRelation::relation
     * @return
     */
    Relationship::SharedRelation AddRelation::relation() const
    {
        return m_Relation;
    }

} // namespace Commands

