/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 26/07/2015.
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
#include "entityhelpres.h"

#include <QStringList>

#include <boost/range/algorithm/find_if.hpp>

#include <entity/scope.h>

#include "constants.h"

using namespace boost;

namespace entity {

    SharedScope chainScopeSearch(const entity::Scopes& scopes, const QStringList &scopesNames)
    {
        if (scopesNames.empty())
            return nullptr;

        auto it = range::find_if(scopes, [&](auto &&scope){ return scope->name() == scopesNames.first(); });
        if (it != cend(scopes)) {
            if (scopesNames.count() == 1) {
                return *it;
            } else {
                QStringList tmp(scopesNames);
                tmp.removeFirst();
                return (*it)->chainScopeSearch(tmp);
            }
        } else {
            return nullptr;
        }
    }

    void uniquifyName(common::BasicElement &ent, const QStringList &names)
    {
        EntityID::ValueType counter = ent.id().value() - EntityID::firstFreeID().value();
        while (names.contains(ent.name())) {

            Q_ASSERT(counter != 0);
            QString prevNum = QString::number(counter - 1);
            QString newName = ent.name();
            if (newName.endsWith(prevNum))
                newName = newName.remove(prevNum).trimmed();

            ent.setName(newName + QChar::Space + QString::number(counter));

            ++counter;
        }
    }

} // namespace entity
