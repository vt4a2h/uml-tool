/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 10/03/2016.
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

#include <entity/entityid.h>

namespace common {

    /// Base class for all elements
    class BasicElement : public QObject
    {
        Q_OBJECT

    public:
        explicit BasicElement(const QString &name = "",
                              const entity::EntityID &id = entity::EntityID::nullID());
        BasicElement(const QString &name, const entity::EntityID &scopeId,
                     const entity::EntityID &id);
        BasicElement(const BasicElement &src);
        BasicElement(BasicElement &&src) noexcept = default;
        BasicElement(const QJsonObject &src, QStringList &errorList);

        virtual entity::EntityID id() const;
        virtual void setId(const entity::EntityID &id);

        virtual entity::EntityID scopeId() const;
        virtual void setScopeId(const entity::EntityID &id);

        virtual QString name() const;
        void setName(const QString &name);

        void writeToFile(const QString &fileName) const;
        bool readFromFile(const QString &fileName);

        virtual QJsonObject toJson() const;
        virtual void fromJson(const QJsonObject &src, QStringList &errorList);

        virtual size_t hashType() const noexcept;
        static size_t staticHashType() noexcept;

        virtual QString marker() const noexcept;
        static QString staticMarker() noexcept;

    signals:
        void nameChanged(const QString &oldName, const QString &newName);

    public:
        BasicElement &operator =(const BasicElement &rhs);
        BasicElement &operator =(BasicElement &&rhs) noexcept = default;

        friend bool operator ==(const BasicElement &lhs, const BasicElement &rhs);

    protected:
        QString m_Name;
        entity::EntityID m_Id;
        entity::EntityID m_ScopeId;
    };

} // namespace common
