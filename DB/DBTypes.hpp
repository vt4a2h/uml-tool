/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 10/05/2015.
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

#include <memory>

#include <QPair>
#include <QVector>
#include <QPointF>
#include <QString>
#include <QMetaType>

#include <Common/ID.h>

namespace DB {
    class Database;
    class ProjectDatabase;
    using SharedDatabase = std::shared_ptr<Database>;
    using WeakDatabase = std::weak_ptr<Database>;
    using SharedProjectDatabase = std::shared_ptr<ProjectDatabase>;
    using WeakProjectDatabase = std::weak_ptr<ProjectDatabase>;

    using IDPos = QPair<Common::ID, QPointF>;
    using ItemsPos = QVector<IDPos>;

    class ITypeSearcher;
    using SharedTypeSearcher = std::shared_ptr<ITypeSearcher>;
    using WeakTypeSearcher = std::weak_ptr<ITypeSearcher>;
    using WeakTypeSearchers = QVector<WeakTypeSearcher>;
    using WeakTypeSearchersSet = QSet<WeakTypeSearcher>;
    using SharedTypeSearchers = QVector<SharedTypeSearcher>;

    class IScopeSearcher;
    using SharedScopeSearcher = std::shared_ptr<IScopeSearcher>;
    using WeakScopeSearcher = std::weak_ptr<IScopeSearcher>;
    using WeakScopeSearchers = QVector<WeakScopeSearcher>;
}

Q_DECLARE_METATYPE(DB::IDPos)
Q_DECLARE_METATYPE(DB::SharedScopeSearcher)
