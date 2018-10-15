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

#include <Common/BasicElement.h>
#include <Common/meta.h>

#include <Entity/Components/icomponents.h> // TODO: remove
#include <Entity/Converters/ConvertersTypes.hpp>
#include <Entity/EntityTypes.hpp>
#include <Entity/ITextRepresentable.hpp>

class QJsonObject;

namespace Entity {

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

    /// Data displayed when element is active
    struct DisplayData
    {
        QString sectionName; ///< The name of section
        QString description; ///< Section description
    };

    /**
     * @brief The Type class
     */
    class Type
        : public Common::BasicElement
        , public ITextRepresentable
        , public IComponents // FIXME: remove!
    {
    public:
        Type();
        Type(const QString &name, const Common::ID &scopeId,
             const Common::ID &typeId = Common::ID::nullID());
        Type(Type &&) noexcept = default;
        Type(const Type &src);

        Type& operator= (Type &&) noexcept = default;
        Type& operator= (const Type &src);

        friend bool operator ==(const Type &lhs, const Type &rhs);

        virtual bool isEqual(const Type &rhs, bool withTypeid = true) const;

        static QString kindMarker();

        KindOfType kindOfType() const;

        SharedGraphicEntityData graphicEntityData() const;
        void setGraphicEntityData(const SharedGraphicEntityData &graphicEntityData);

        friend void swap(Type &lhs, Type &rhs) noexcept;

        virtual OptionalDisplayData displayData() const;

        template <class DstElem>
        const DstElem &to() const noexcept
        {
            Q_ASSERT(hashType() == DstElem::staticHashType());
            return static_cast<const DstElem &>(*this);
        }

        template <class DstElem>
        DstElem &to() noexcept
        {
            return const_cast<DstElem &>(const_cast<const Type *>(this)->to<DstElem>());
        }

    public: // ITextRepresentable implementetion
        QString toString() const noexcept override;
        bool fromString(const QString &s) noexcept override;
        void setTextConversionStrategy(const Converters::SharedConversionStrategy &convStrat) override;

    public: // BasicElement implementation
        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        add_meta(Type)

    protected:
        KindOfType m_KindOfType;

    private:
        void setBaseTypeName();

        SharedGraphicEntityData m_GraphicEntityData;

        Converters::SharedConversionStrategy m_TextConversionStrategy;
    };

} // namespace entity
