/*****************************************************************************
**
** Copyright (C) 2017 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 21.
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

#include <QPointer>

#include <Entity/EntityTypes.hpp>
#include <Models/ModelsTypes.hpp>

QT_BEGIN_NAMESPACE
class QLayout;
QT_END_NAMESPACE

namespace GUI {

    /// Handle Entities properties in \c Sections added to the specific layout
    class IPropertiesHandler
    {
    public: // Methods
        virtual ~IPropertiesHandler() = default;

        /// Display properties
        virtual bool activate() = 0;

        /// Hide properties
        virtual bool deactivate() = 0;

        /// Set object for diaplying properties
        virtual void setEntity(const Entity::SharedType &type) = 0;

        /// Check if current changes are saved
        virtual bool isSaved() const = 0;

        /// Save current changes
        virtual void save() = 0;
    };

} // namespace GUI
