/*****************************************************************************
**
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 29/10/2014.
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

#include "type.h"

#include <utility>
#include <QHash>
#include <QStringList>

namespace entity {

    /// The Vsriable class
    struct Variable : public QPair<QString, int>
    {
        Variable() : QPair<QString, int>() {}
        Variable(const QString &id, int value) : QPair<QString, int>(id, value) {}
    };

    /// The Enum class
    class Enum : public Type
    {
    public:
        Enum();
        Enum(const QString &name, const QString &scopeId);

        friend bool operator== (const Enum &lhs, const Enum &rhs);

        bool isStrong() const;
        void setStrongStatus(bool status);

        Variable &addVariable(const QString &name);
        Variable getVariable(const QString &name) const;
        void removeVariable(const QString &name);
        bool containsVariable(const QString &name);
        VariablesList variables() const override;

        QString enumTypeId() const;
        void setEnumTypeId(const QString &enumTypeId);

        bool isEqual(const Enum &rhs) const;

    public: // BasicEntity implementation
        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        size_t hashType() const override;
        static size_t staticHashType();

        QString marker() const override;
        static QString staticMarker();

    private:
        QString m_EnumTypeId;
        bool m_StrongStatus;
        VariablesList m_Variables;
    };

} // namespace entity
