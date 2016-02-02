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

#include <QHash>
#include <QStringList>

namespace entity {

    class EntityID;

    /// The Variable class
    class Element : public BasicEntity
    {
    public:
        Element();
        Element(const QString &name, int value);
        Element(const QJsonObject &src, QStringList &errorList);

        friend bool operator ==(const Element &lhs, const Element &rhs);

        int value() const;
        void setValue(int value);

    public: // BasicEntity implementation
        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        size_t hashType() const override;
        static size_t staticHashType();

    private:
        int m_Value;
    };

    /// The Enum class
    class Enum : public Type
    {
    public:
        Enum();
        Enum(const QString &name, const EntityID &scopeId);

        friend bool operator== (const Enum &lhs, const Enum &rhs);

        bool isStrong() const;
        void setStrongStatus(bool status);

        SharedElement addElement(const QString &name);
        SharedElement element(const QString &name) const;
        void removeElement(const QString &name);
        bool containsElement(const QString &name) const;

        QString enumTypeId() const;
        void setEnumTypeId(const QString &enumTypeId);

        bool isEqual(const Type &rhs, bool withTypeid = true) const override;

    public: // IComponents implmentaion
        SharedElement addNewElement() override;
        void addExistsElement(const SharedElement &element, int pos = -1) override;
        int removeElement(const SharedElement &element) override;
        ElementsList elements() const override;

    public: // BasicEntity implementation
        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        size_t hashType() const override;
        static size_t staticHashType();

        QString marker() const override;
        static QString staticMarker();

        QString defaultName() const override;
        static QString staticDefaultName();

    private:
        QString m_EnumTypeId;
        bool m_StrongStatus;
        ElementsList m_Elements;
    };

} // namespace entity

Q_DECLARE_METATYPE(entity::Element)
Q_DECLARE_METATYPE(entity::SharedElement)
