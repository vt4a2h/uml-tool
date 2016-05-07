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

#include <QString>

#include <common/BasicElement.h>
#include <common/meta.h>

#include <entity/components/icomponents.h>

#include "entity_types.hpp"

class QJsonObject;

namespace entity {

    class Scope;

    /// The type of type :)
    enum class KindOfType : int
    {
        Type,          ///< Type
        ExtendedType,  ///< ExtendedType
        Enum,          ///< Enum
        Union,         ///< Union
        Class,         ///< Class
        TemplateClass, ///< TemplateClass
    };

    uint qHash(const KindOfType &e);
    QString kindOfTypeToString(KindOfType kind, bool raw = true);

    /**
     * @brief The Type class
     */
    class Type : public common::BasicElement, public IComponents
    {
    public:
        Type();
        Type(const QString &name, const common::ID &scopeId,
             const common::ID &typeId = common::ID::nullID());
        Type(Type &&) noexcept = default;
        Type(const Type &src);

        Type& operator= (Type &&) noexcept = default;
        Type& operator= (Type src);

        friend bool operator ==(const Type &lhs, const Type &rhs);

        virtual bool isEqual(const Type &rhs, bool withTypeid = true) const;

        static QString kindMarker();

        KindOfType kindOfType() const;

        SharedGraphicEntityData graphicEntityData() const;
        void setGraphicEntityData(const SharedGraphicEntityData &graphicEntityData);

        friend void swap(Type &lhs, Type &rhs) noexcept;

    public: // BasicEntity implementation
        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        add_meta(Type)

    protected:
        KindOfType m_KindOfType;

    private:
        void setBaseTypeName();

        SharedGraphicEntityData m_GraphicEntityData;
    };

} // namespace entity
