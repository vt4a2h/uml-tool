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

#include <QHash>
#include <QStringList>
#include <QVector>

#include "Type.h"
#include "isectional.h"

namespace Entity {

    class ID;

    /// The Variable class
    class Enumerator
        : public Common::BasicElement
        , public ISectional
    {
    public: // Types
        enum ValueBase { Dec = 10, Oct = 8, Hex = 16 };

        using Value = std::pair<int, ValueBase>;
        using OptionalValue = std::optional<Value>;

    public:
        Enumerator();
        Enumerator(const QString &name, OptionalValue value = std::nullopt);
        Enumerator(const QJsonObject &src, QStringList &errorList);
        Enumerator(Enumerator &&) noexcept = default;
        Enumerator(const Enumerator &) = default;

        Enumerator& operator= (Enumerator &&) noexcept = default;
        Enumerator& operator= (const Enumerator &) = default;
        friend bool operator ==(const Enumerator &lhs, const Enumerator &rhs);

        OptionalValue value() const;
        void setValue(const OptionalValue &value);
        void setValue(int value);

        static QString valToString(Value const& v);
        static Value valFromString(QString s);

    public: // BasicEntity implementation
        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        add_meta(Enumerator)

    private:
        OptionalValue m_Value;
    };

    /// The Enum class
    class Enum : public Type
    {
    public:
        Enum();
        Enum(const QString &name, const Common::ID &scopeId);
        Enum(Enum &&src) noexcept;
        Enum(const Enum & src);

        Enum& operator= (Enum &&src) noexcept;
        Enum& operator= (const Enum &src);
        friend bool operator== (const Enum &lhs, const Enum &rhs);

        bool isStrong() const;
        void setStrongStatus(bool status);

        SharedEnumarator addElement(const QString &name);
        SharedEnumarator element(const QString &name) const;
        void removeEnumerator(const QString &name);
        bool containsElement(const QString &name) const;

        Common::ID enumTypeId() const;
        void setEnumTypeId(const Common::ID &enumTypeId);

        bool isEqual(const Type &rhs, bool withTypeid = true) const override;

        friend void swap(Enum &lhs, Enum &rhs) noexcept;

    public: // IComponents implmentaion
        SharedEnumarator addNewEnumerator() override;
        void addExistsEnumerator(const SharedEnumarator &element, int pos = -1) override;
        int removeEnumerator(const SharedEnumarator &element) override;
        Enumerators enumerators() const override;

    public: // BasicEntity implementation
        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        add_meta(Enum)

    private:
        Common::ID m_EnumTypeId;
        bool m_StrongStatus;
        Enumerators m_Elements;
    };

} // namespace entity

Q_DECLARE_METATYPE(Entity::Enumerator)
Q_DECLARE_METATYPE(Entity::SharedEnumarator)
