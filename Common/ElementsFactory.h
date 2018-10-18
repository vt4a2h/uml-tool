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
#pragma once

#include <QObject>
#include <QPointer>

#include <DB/DBTypes.hpp>

#include <Project/ProjectTypes.hpp>

#include <Models/ModelsTypes.hpp>

#include <Entity/EntityTypes.hpp>

#include <Commands/CommandsTypes.h>

class QGraphicsScene;

namespace Common {

    /// The base class for schema elements factories
    class ElementsFactory : public QObject
    {
        Q_OBJECT

    public: // Types

        /// Creation options
        enum CreationOption {
            NoOptions      = 0x0, ///< Empty
            AddToScene     = 0x1, ///< Add graphic representation of item to the scene
            AddToTreeModel = 0x2, ///< Add item to the tree model
            AddToDatabase  = 0x4, ///< Add to the project database, actual only for relations
            EntityCommon   = AddToScene | AddToTreeModel,
            RelationCommon = AddToScene | AddToTreeModel | AddToDatabase,
        };
        Q_DECLARE_FLAGS(CreationOptions, CreationOption)

    public:
        explicit ElementsFactory(QObject *parent = nullptr);

        DB::SharedDatabase globalDatabase() const;
        void setGlobalDatabase(const DB::SharedDatabase &db);

        QPointer<QGraphicsScene> scene() const;
        DB::SharedDatabase db() const;
        Projects::SharedProject project() const;

        Models::SharedTreeModel treeModel() const;
        void setTreeModel(const Models::WeakTreeModel &treeModel);

        Commands::SharedCommandStack commandStack() const;
        void setCommandStack(const Commands::SharedCommandStack &commandStack);

        virtual void init();

        Models::SharedMessenger messenger() const;
        void setMessenger(const Models::SharedMessenger &messenger);

    public slots:
        void onSceneChanged(const QPointer<QGraphicsScene> &scene);
        void onProjectChanged(const Projects::SharedProject &p, const Projects::SharedProject &c);
        void addAdditionaScopeSearcher(const DB::SharedScopeSearcher &s);
        void removeAdditionaScopeSearcher(const DB::SharedScopeSearcher &s);

    signals:
        void projectChanged(const Projects::SharedProject &p, const Projects::SharedProject &c);

    protected:
        Entity::SharedScope scope(const Common::ID &id) const;

    private:
        QPointer<QGraphicsScene> m_Scene;
        Projects::WeakProject m_Project;
        DB::WeakDatabase m_GlobalDatabase;
        DB::WeakScopeSearchers m_AdditionalScopeSearchers;
        Models::WeakTreeModel m_TreeModel;
        Commands::SharedCommandStack m_CommandStack;
        Models::SharedMessenger m_Messenger;
    };

} // namespace common

Q_DECLARE_OPERATORS_FOR_FLAGS(Common::ElementsFactory::CreationOptions)
