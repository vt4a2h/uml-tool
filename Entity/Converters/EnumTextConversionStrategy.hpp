/*****************************************************************************
**
** Copyright (C) 2018 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 17/10/2018.
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

#include <Entity/Converters/BaseTextConversionStrategy.hpp>

namespace Entity::Converters {

    class EnumTextConversionStrategy: public BaseTextConversionStrategy
    {
    protected: // ITextConversionStrategy interface
        QString toStringImpl(const Type &element) const override;
        bool fromStringImpl(const QString &s, Type &element) const override;

    private:
        void addEnumTypename(const Entity::Enum &e, QString &result) const;
    };

} // namespace Entity::Converters
