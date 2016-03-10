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

#include "enum.h"

#include <algorithm>
#include <utility>

#include <QJsonObject>
#include <QStringList>
#include <QJsonArray>

#include <boost/range/algorithm/find_if.hpp>

#include <utility/helpfunctions.h>

#include "constants.h"
#include "enums.h"
#include "entityid.h"

using namespace boost;

namespace entity {

    namespace {
        const QString newElementName = Enumerator::tr("newElement");
        const QString nameMark   = "Name";
        const QString numberMark = "Number";
    }

    /**
     * @brief Element::Element
     */
    Enumerator::Enumerator()
        : Enumerator("Enumeration", 0)
    {}

    /**
     * @brief Element::Element
     * @param name
     * @param value
     */
    Enumerator::Enumerator(const QString &name, int value)
        : BasicElement(name)
        , m_Value(value)
    {}

    /**
     * @brief Variable::toJson
     * @return
     */
    Enumerator::Enumerator(const QJsonObject &src, QStringList &errorList)
        : BasicElement(src, errorList)
        , m_Value(0)
    {
        fromJson(src, errorList);
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const Enumerator &lhs, const Enumerator &rhs)
    {
        return static_cast<BasicElement const&>(lhs) == static_cast<BasicElement const&>(rhs) &&
               lhs.m_Value == rhs.m_Value;
    }

    /**
     * @brief Element::toJson
     * @return
     */
    QJsonObject Enumerator::toJson() const
    {
        QJsonObject result = BasicElement::toJson();
        result.insert(numberMark, m_Value);

        return result;
    }

    /**
     * @brief Variable::fromJson
     * @param src
     * @param errorList
     */
    void Enumerator::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        BasicElement::fromJson(src, errorList);
        utility::checkAndSet(src, numberMark, errorList, [&src, this](){ m_Value = src[numberMark].toInt();  });
    }

    /**
     * @brief Variable::hashType
     * @return
     */
    size_t Enumerator::hashType() const
    {
        return Enumerator::staticHashType();
    }

    /**
     * @brief Variable::staticHashType
     * @return
     */
    size_t Enumerator::staticHashType()
    {
        return typeid(Enumerator).hash_code();
    }

    /**
     * @brief Element::value
     * @return
     */
    int Enumerator::value() const
    {
        return m_Value;
    }

    /**
     * @brief Element::setValue
     * @param value
     */
    void Enumerator::setValue(int value)
    {
        m_Value = value;
    }

    /**
     * @brief Enum::Enum
     */
    Enum::Enum()
        : Enum(defaultName(), EntityID::globalScopeID())
    {
    }

    /**
     * @brief Enum::Enum
     * @param name
     * @param scopeId
     */
    Enum::Enum(const QString &name, const EntityID &scopeId)
        : Type(name, scopeId)
        , m_EnumTypeId(EntityID::nullID())
        , m_StrongStatus(false)
    {
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator==(const Enum &lhs, const Enum &rhs)
    {
        return lhs.isEqual(rhs);
    }

    /**
     * @brief Enum::isStrong
     * @return
     */
    bool Enum::isStrong() const
    {
        return m_StrongStatus;
    }

    /**
     * @brief Enum::setStrongStatus
     * @param status
     */
    void Enum::setStrongStatus(bool status)
    {
        m_StrongStatus = status;
    }

    /**
     * @brief Enum::addVariable
     * @param name
     * @return
     */
    SharedEnumarator Enum::addElement(const QString &name)
    {
        auto element = std::make_shared<Enumerator>(name, m_Elements.size());
        m_Elements << element;
        return element;
    }

    /**
     * @brief Enum::getVariable
     * @param name
     * @return
     */
    SharedEnumarator Enum::element(const QString &name) const
    {
        auto it = range::find_if(m_Elements, [&](auto &&e){ return e->name() == name; });
        return it != cend(m_Elements) ? *it : SharedEnumarator();
    }

    /**
     * @brief Enum::removeVariable
     * @param name
     */
    void Enum::removeEnumerator(const QString &name)
    {
        auto it = range::find_if(m_Elements, [&](auto &&v){ return v->name() == name; });
        if (it != m_Elements.end())
            m_Elements.erase(it);
    }

    /**
     * @brief Enum::containsVariable
     * @param name
     * @return
     */
    bool Enum::containsElement(const QString &name) const
    {
        return range::find_if(m_Elements, [&](auto &&v){ return v->name() == name; })
               != cend(m_Elements);
    }

    /**
     * @brief Enum::variables
     * @return
     */
    Enumerators Enum::enumerators() const
    {
        return m_Elements;
    }

    /**
     * @brief Enum::enumTypeId
     * @return
     */
    EntityID Enum::enumTypeId() const
    {
        return m_EnumTypeId;
    }

    /**
     * @brief Enum::setEnumTypeId
     * @param enumTypeId
     */
    void Enum::setEnumTypeId(const EntityID &enumTypeId)
    {
        m_EnumTypeId = enumTypeId;
    }

    /**
     * @brief Enum::toJson
     * @return
     */
    QJsonObject Enum::toJson() const
    {
        QJsonObject result(Type::toJson());

        result.insert("Enum type id", m_EnumTypeId.toJson());
        result.insert("Strong status", m_StrongStatus);

        QJsonArray elements;
        for (auto &&v : m_Elements)
            elements.append(v->toJson());

        result.insert("Elements", elements);

        return result;
    }

    /**
     * @brief Enum::fromJson
     * @param src
     * @param errorList
     */
    void Enum::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        Type::fromJson(src, errorList);

        utility::checkAndSet(src, "Enum type id",  errorList, [&, this](){ m_EnumTypeId.fromJson(src["Enum type id"], errorList); });
        utility::checkAndSet(src, "Strong status", errorList, [&src, this](){ m_StrongStatus = src["Strong status"].toBool();  });

        m_Elements.clear();
        utility::checkAndSet(src, "Elements", errorList, [&src, &errorList, this](){
            if (src["Elements"].isArray()) {
                for (auto &&value : src["Elements"].toArray())
                    m_Elements.append(std::make_shared<Enumerator>(value.toObject(), errorList));
            } else {
                errorList << "Error: \"Elements\" is not array";
            }
        });
    }

    /**
     * @brief Enum::hashType
     * @return
     */
    size_t Enum::hashType() const
    {
        return Enum::staticHashType();
    }

    /**
     * @brief Enum::staticHashType
     * @return
     */
    size_t Enum::staticHashType()
    {
        return typeid(Enum).hash_code();
    }

    /**
     * @brief Enum::marker
     * @return
     */
    QString Enum::marker() const
    {
        return Enum::staticMarker();
    }

    /**
     * @brief Enum::staticMarker
     * @return
     */
    QString Enum::staticMarker()
    {
        return "Enum";
    }

    /**
     * @brief Enum::defaultName
     * @return
     */
    QString Enum::defaultName() const
    {
        return staticDefaultName();
    }

    /**
     * @brief Enum::staticDefaultName
     * @return
     */
    QString Enum::staticDefaultName()
    {
       return tr("Enumeration");
    }

    /**
     * @brief Enum::isEqual
     * @param rhs
     * @return
     */
    bool Enum::isEqual(const Type &rhs, bool withTypeid) const
    {
        if (!Type::isEqual(rhs, withTypeid))
            return false;

        auto r = static_cast<const Enum&>(rhs);
        return m_EnumTypeId   == r.m_EnumTypeId   &&
               m_StrongStatus == r.m_StrongStatus &&
               utility::seqSharedPointerEq(m_Elements, r.m_Elements);
    }

    /**
     * @brief Enum::addNewElement
     * @return
     */
    SharedEnumarator Enum::addNewEnumerator()
    {
        return addElement(newElementName + QString::number(m_Elements.count()));
    }

    /**
     * @brief Enum::addExistsElement
     * @param element
     * @param pos
     */
    void Enum::addExistsEnumerator(const SharedEnumarator &element, int pos)
    {
        m_Elements.insert(pos, element);
    }

    /**
     * @brief Enum::removeElement
     * @param element
     * @return
     */
    int Enum::removeEnumerator(const SharedEnumarator &element)
    {
        int pos = m_Elements.indexOf(element);
        m_Elements.removeAt(pos);
        return pos;
    }

} // namespace entity
