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
#include "types.h"
#include "entity_types.hpp"

#include <QString>
#include <QList>

namespace entity {

    /**
     * @brief The ExtendedType class
     */
    class ExtendedType : public Type
    {
    public:
        using Pl     = std::pair<QString, bool>; // (pointer or link, const for pointer)
        using PlList = QList<Pl>;

        ExtendedType();
        ExtendedType(const QString &name, const QString &scopeId);

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

        void addTemplateParameter(const QString &typeId);
        bool containsTemplateParameter(const QString &typeId) const;
        void removeTemplateParameters(const QString &typeId);
        ExtendedTypesIdList templateParameters() const;

        QString typeId() const;
        void setTypeId(const QString &typeId);

        bool isEqual(const Type &rhs, bool withTypeid = true) const override;

    public: // BasicEntity implementation
        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        size_t hashType() const override;
        static size_t staticHashType();

        QString marker() const override;
        static QString staticMarker();

    protected:
        bool    m_ConstStatus;
        QString m_TypeId;
        PlList  m_PointersAndLinks;

        ExtendedTypesIdList m_TemplateParameters;
    };

} // namespace entity
