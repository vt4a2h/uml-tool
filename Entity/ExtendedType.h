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

#include "Type.h"
#include "types.h"
#include "EntityTypes.hpp"

#include <QString>
#include <QVector>

namespace Entity {

    /**
     * @brief The ExtendedType class
     */
    class ExtendedType : public Type
    {
    public:
        // TODO: no QString, use enum to indicate * or &
        using Pl     = std::pair<QString, bool>; // (pointer or link, const for pointer)
        using PlList = QList<Pl>;

        ExtendedType();
        ExtendedType(const QString &name, const Common::ID &scopeId);
        ExtendedType(ExtendedType &&) noexcept = default;
        ExtendedType(const ExtendedType &) = default;

        ExtendedType& operator= (ExtendedType &&) noexcept = default;
        ExtendedType& operator= (const ExtendedType &) = default;
        friend bool operator ==(const ExtendedType &lhs, const ExtendedType &rhs);

        bool isPointer() const;
        void addPointerStatus(bool pointerToConst = false);
        void removePointerStatus();

        bool isLink() const;
        void addLinkStatus();
        void removeLinkStatus();

        PlList pl() const;

        bool isConst() const;
        void setConstStatus(bool status);

        void addTemplateParameter(const Common::ID &typeId);
        bool containsTemplateParameter(const Common::ID &typeId) const;
        void removeTemplateParameters(const Common::ID &typeId);
        ExtendedTypesIdList templateParameters() const;

        Common::ID typeId() const;
        void setTypeId(const Common::ID &typeId);

        bool useAlias() const;
        void setUseAlias(bool useAlias);

        bool isEqual(const Type &rhs, bool withTypeid = true) const override;

    public: // BasicEntity implementation
        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        add_meta(ExtendedType)

    protected:
        bool       m_ConstStatus;
        bool       m_UseAlias;
        Common::ID m_TypeId;
        PlList     m_PointersAndLinks;

        ExtendedTypesIdList m_TemplateParameters;
    };

} // namespace entity
