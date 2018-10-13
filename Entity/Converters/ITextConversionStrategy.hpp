/*****************************************************************************
**
** Copyright (C) 2018 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 14/10/2018.
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

#include <DB/DBTypes.hpp>
#include <Models/ModelsTypes.hpp>

namespace Entity::Converters {

    /// Convert basic objects from text and vice versa
    template <class Element>
    class ITextConversionStrategy
    {
    public:
        virtual ~ITextConversionStrategy() = default;

        /// Convert element to the text representation
        /**
         * @return QString::null if conversion failed, and text representation otherwise
         */
        virtual QString toString(const Element &element) const noexcept = 0;

        /// Attempt to parse @p element from string @p s
        /**
         * If conversion failed, element is unchanged
         * @return true if successed, false if failed
         */
        virtual bool fromString(const QString &s, Element &element) const noexcept = 0;

        /// Register any useful type searcher, e.g., project database
        /**
         * Also removes all stale type searchers
         * @note This is mandatory for some conversions, e.g., to convert property
         */
        virtual void registerTypeSearcher(const DB::WeakTypeSearcher &typeSearcher) = 0;

        /// Typesearcher will never be used for searching types anymore
        virtual void unregisterTypeSearcher(const DB::WeakTypeSearcher &typeSearcher) = 0;

        /// Register an object for logging messages
        /**
         * @note if there is no messenger registered, user will not be able to track conversion state
         */
        virtual void registerMessenger(const Models::SharedMessenger &messenger) = 0;
    };

} // namespace Entity::Converters
