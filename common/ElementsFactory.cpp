/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 31/03/2016.
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
#include "ElementsFactory.h"

#include <boost/range/algorithm/find.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

#include <db/ProjectDatabase.h>
#include <project/Project.h>

namespace common {

    using namespace boost;

    /**
     * @brief ElementsFactory::ElementsFactory
     * @param parent
     */
    ElementsFactory::ElementsFactory(QObject *parent)
        : QObject(parent)
    {
    }

    /**
     * @brief ElementsFactory::setGlobalDatabase
     * @param db
     */
    void ElementsFactory::setGlobalDatabase(const db::SharedDatabase &db)
    {
        m_GlobalDatabase = db;
    }

    /**
     * @brief ElementsFactory::onSceneChanged
     * @param scene
     */
    void ElementsFactory::onSceneChanged(const QPointer<QGraphicsScene> &scene)
    {
        m_Scene = scene;
    }

    /**
     * @brief ElementsFactory::onProjectChanged
     * @param p
     */
    void ElementsFactory::onProjectChanged(const project::SharedProject &p)
    {
        m_Project = p;
    }

    /**
     * @brief ElementsFactory::addAdditionaScopeSearcher
     * @param s
     */
    void ElementsFactory::addAdditionaScopeSearcher(const db::SharedScopeSearcher &s)
    {
        Q_ASSERT(range::find(m_AdditionalScopeSearchers, s) == m_AdditionalScopeSearchers.end());
        m_AdditionalScopeSearchers << s;
    }

    /**
     * @brief ElementsFactory::removeAdditionaScopeSearcher
     * @param s
     */
    void ElementsFactory::removeAdditionaScopeSearcher(const db::SharedScopeSearcher &s)
    {
        Q_ASSERT(range::find(m_AdditionalScopeSearchers, s) != m_AdditionalScopeSearchers.end());
        range::remove_erase(m_AdditionalScopeSearchers, s);
    }

    /**
     * @brief ElementsFactory::scope
     * @param id
     * @return
     */
    entity::SharedScope ElementsFactory::scope(const ID &id) const
    {
        entity::SharedScope scope;
        if (auto database = db()) {
            // Try to search in the global or project database
            scope = database->scope(id, true /*searchInDepth*/);

            // If is not found, check additional scope searchers
            if (!scope) {
                for (auto &&searcher : m_AdditionalScopeSearchers) {
                    if (auto sharedSearcher = searcher.lock())
                        scope = sharedSearcher->scope(id, true /*searchInDepth*/);

                    if (scope)
                        break;
                }
            }
        }

        return scope;
    }

    /**
     * @brief ElementsFactory::scene
     * @return
     */
    QPointer<QGraphicsScene> ElementsFactory::scene() const
    {
        return m_Scene;
    }

    /**
     * @brief ElementsFactory::db
     * @return
     */
    db::SharedDatabase ElementsFactory::db() const
    {
        return project() ? project()->database() : m_GlobalDatabase.lock();
    }

    /**
     * @brief ElementsFactory::project
     * @return
     */
    project::SharedProject ElementsFactory::project() const
    {
        return m_Project.lock();
    }


    /**
     * @brief EntityFactory::treeModel
     * @return
     */
    models::SharedTreeModel ElementsFactory::treeModel() const
    {
        return m_TreeModel.lock();
    }

    /**
     * @brief EntityFactory::setTreeModel
     * @param treeModel
     */
    void ElementsFactory::setTreeModel(const models::WeakTreeModel &treeModel)
    {
        m_TreeModel = treeModel;
    }

} // namespace common
