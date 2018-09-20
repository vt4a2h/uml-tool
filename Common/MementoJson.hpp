/*****************************************************************************
**
** Copyright (C) 20186 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 20/09/2018.
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

#include <QJsonObject>

#include "IMemento.hpp"
#include "CommonTypes.hpp"

namespace Common {

    class MementoJson : public IMemento
    {
    public:
        MementoJson(const BasicElement &elem);

        Type type() const override;

        QJsonObject json() const;

    private:
        QJsonObject m_JsonObj;
    };

} // namespace Common
