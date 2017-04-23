/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 31/01/2016.
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

#include <QtGlobal>
#include <QJsonValue>
#include <QMetaType>
#include <QCoreApplication>

#include <boost/operators.hpp>

#include "types.h"

namespace common
{

    /// ID for all objects
    class ID : public boost::equality_comparable<ID>
             , public boost::less_than_comparable<ID>
    {
        Q_DECLARE_TR_FUNCTIONS(ID)

    public:
        using ValueType = quint64;

        ID();
        explicit ID(quint64 value);
        ID(ID const&);
        ID(ID &&) noexcept;

        ID &operator =(ID const&);
        ID &operator =(ID &&) noexcept;

        ~ID() = default;

        friend bool operator ==(const ID &lhs, const ID &rhs);
        friend bool operator < (const ID &lhs, const ID &rhs);

        friend ID operator +(const ID &lhs, const ID &rhs);
        friend ID operator +(const ID &lhs, int val);

        ID operator ++(int);

        bool isValid() const;

        QString toString() const;
        bool fromString(const QString &in);

        quint64 value() const;
        void setValue(const quint64 &value);

        QJsonValue toJson() const;
        void fromJson(const QJsonValue &in, ErrorList & errors);

        friend void swap(ID & lsh, ID & rhs);

    public: // Constants
        static ID nullID();

        static ID firstFreeID();
        static ID firstNonConstID();

        // Keep order please, add new item to the end. Next item value =
        // previous item value + 1, e.g. stdScopeID = globalScopeID + 1
        static ID globalScopeID();
        static ID stdScopeID();
        static ID globalDatabaseID();
        static ID localTemplateScopeID();
        static ID projectScopeID();

    private:
        ValueType m_value;
    };

    uint qHash(const ID &e);
} // namespace common

Q_DECLARE_METATYPE(common::ID)
