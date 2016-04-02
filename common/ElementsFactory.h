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

#include <db/db_types.hpp>

#include <project/project_types.hpp>

class QGraphicsScene;

namespace common {

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
        };
        Q_DECLARE_FLAGS(CreationOptions, CreationOption)

    public:
        explicit ElementsFactory(QObject *parent = 0);

    public slots:
        void onSceneChanged(const QPointer<QGraphicsScene> &scene);
        void onProjectChanged(const project::SharedProject &p);

    protected:
        QPointer<QGraphicsScene> scene() const;
        db::SharedProjectDatabase db() const;
        project::SharedProject project() const;

    private:
        QPointer<QGraphicsScene> m_Scene;
        project::WeakProject m_Project;
    };

} // namespace common

Q_DECLARE_OPERATORS_FOR_FLAGS(common::ElementsFactory::CreationOptions)
