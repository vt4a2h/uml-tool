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

#include <DB/ProjectDatabase.h>
#include <Project/Project.h>

namespace Common {

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
     * @param DB
     */
    void ElementsFactory::setGlobalDatabase(const DB::SharedDatabase &db)
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
    void ElementsFactory::onProjectChanged(const Projects::SharedProject &,
                                           const Projects::SharedProject &c)
    {
        m_Project = c;
    }

    /**
     * @brief ElementsFactory::addAdditionaScopeSearcher
     * @param s
     */
    void ElementsFactory::addAdditionaScopeSearcher(const DB::SharedScopeSearcher &s)
    {
        Q_ASSERT(!m_AdditionalScopeSearchers.contains(s));
        m_AdditionalScopeSearchers << s;
    }

    /**
     * @brief ElementsFactory::removeAdditionaScopeSearcher
     * @param s
     */
    void ElementsFactory::removeAdditionaScopeSearcher(const DB::SharedScopeSearcher &s)
    {
        Q_ASSERT(m_AdditionalScopeSearchers.contains(s));
        m_AdditionalScopeSearchers.remove(m_AdditionalScopeSearchers.indexOf(s));
    }

    /**
     * @brief ElementsFactory::scope
     * @param id
     * @return
     */
    Entity::SharedScope ElementsFactory::scope(const ID &id) const
    {
        Entity::SharedScope scope;
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
     * @brief ElementsFactory::commandStack
     * @return
     */
    Commands::SharedCommandStack ElementsFactory::commandStack() const
    {
        return m_CommandStack;
    }

    /**
     * @brief ElementsFactory::setCommandStack
     * @param commandStack
     */
    void ElementsFactory::setCommandStack(const Commands::SharedCommandStack &commandStack)
    {
        m_CommandStack = commandStack;
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
    DB::SharedDatabase ElementsFactory::db() const
    {
        return project() ? project()->database() : m_GlobalDatabase.lock();
    }

    /**
     * @brief ElementsFactory::project
     * @return
     */
    Projects::SharedProject ElementsFactory::project() const
    {
        return m_Project.lock();
    }


    /**
     * @brief EntityFactory::treeModel
     * @return
     */
    Models::SharedTreeModel ElementsFactory::treeModel() const
    {
        return m_TreeModel.lock();
    }

    /**
     * @brief EntityFactory::setTreeModel
     * @param treeModel
     */
    void ElementsFactory::setTreeModel(const Models::WeakTreeModel &treeModel)
    {
        m_TreeModel = treeModel;
    }

} // namespace common
