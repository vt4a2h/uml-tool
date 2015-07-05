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
#include "constants.h"
#include "enums.h"

#include <algorithm>
#include <utility>

#include <QJsonObject>
#include <QStringList>
#include <QJsonArray>

#include <utility/helpfunctions.h>

namespace entity {

    namespace {
        const QString newElementName = Element::tr("newElement");
    }

    /**
     * @brief Variable::toJson
     * @return
     */
    QJsonObject Element::toJson() const
    {
        QJsonObject result;
        result.insert("Name", first);
        result.insert("Number", second);

        return result;
    }

    /**
     * @brief Variable::fromJson
     * @param src
     * @param errorList
     */
    void Element::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        utility::checkAndSet(src, "Name",   errorList, [&src, this](){ first  = src["Name"].toString(); });
        utility::checkAndSet(src, "Number", errorList, [&src, this](){ second = src["Number"].toInt();  });
    }

    /**
     * @brief Variable::hashType
     * @return
     */
    size_t Element::hashType() const
    {
        return Element::staticHashType();
    }

    /**
     * @brief Variable::staticHashType
     * @return
     */
    size_t Element::staticHashType()
    {
        return typeid(Element).hash_code();
    }

    /**
     * @brief Enum::Enum
     */
    Enum::Enum()
        : Enum(DEFAULT_NAME, GLOBAL_SCOPE_ID)
    {
    }

    /**
     * @brief Enum::Enum
     * @param name
     * @param scopeId
     */
    Enum::Enum(const QString &name, const QString &scopeId)
        : Type(name, scopeId)
        , m_EnumTypeId(STUB_ID)
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
        return static_cast<const Type&>(lhs).isEqual(rhs) &&
               lhs.m_EnumTypeId   == rhs.m_EnumTypeId     &&
               lhs.m_StrongStatus == rhs.m_StrongStatus   &&
               utility::seqSharedPointerEq(lhs.m_Elements, rhs.m_Elements);
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
    SharedElement Enum::addElement(const QString &name)
    {
        auto variable = std::make_shared<Element>(name, m_Elements.size());
        m_Elements << variable;
        return variable;
    }

    /**
     * @brief Enum::getVariable
     * @param name
     * @return
     */
    SharedElement Enum::element(const QString &name) const
    {
        auto it = utility::find_if(m_Elements, [&name](const SharedElement &v){ return v->first == name; });
        return it != m_Elements.cend() ? *it : SharedElement();
    }

    /**
     * @brief Enum::removeVariable
     * @param name
     */
    void Enum::removeElement(const QString &name)
    {
        auto it = utility::find_if(m_Elements, [&name](const SharedElement &v){ return v->first == name; });
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
        return utility::find_if(m_Elements, [&name](const SharedElement &v){ return v->first == name; }) != m_Elements.cend();
    }

    /**
     * @brief Enum::variables
     * @return
     */
    ElementsList Enum::elements() const
    {
        return m_Elements;
    }

    /**
     * @brief Enum::enumTypeId
     * @return
     */
    QString Enum::enumTypeId() const
    {
        return m_EnumTypeId;
    }

    /**
     * @brief Enum::setEnumTypeId
     * @param enumTypeId
     */
    void Enum::setEnumTypeId(const QString &enumTypeId)
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

        result.insert("Enum type id", m_EnumTypeId);
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

        utility::checkAndSet(src, "Enum type id",  errorList, [&src, this](){ m_EnumTypeId   = src["Enum type id"].toString(); });
        utility::checkAndSet(src, "Strong status", errorList, [&src, this](){ m_StrongStatus = src["Strong status"].toBool();  });

        m_Elements.clear();
        utility::checkAndSet(src, "Elements", errorList, [&src, &errorList, this](){
            if (src["Elements"].isArray()) {
                for (auto &&value : src["Elements"].toArray())
                    m_Elements.append(std::make_shared<Element>(value.toObject(), errorList));
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
     * @brief Enum::isEqual
     * @param rhs
     * @return
     */
    bool Enum::isEqual(const Enum &rhs) const
    {
        return *this == rhs;
    }

    /**
     * @brief Enum::addNewElement
     * @return
     */
    SharedElement Enum::addNewElement()
    {
        return addElement(newElementName + QString::number(m_Elements.count()));
    }

    /**
     * @brief Enum::addExistsElement
     * @param element
     * @param pos
     */
    void Enum::addExistsElement(const SharedElement &element, int pos)
    {
        m_Elements.insert(pos, element);
    }

    /**
     * @brief Enum::removeElement
     * @param element
     * @return
     */
    int Enum::removeElement(const SharedElement &element)
    {
        int pos = m_Elements.indexOf(element);
        m_Elements.removeAt(pos);
        return pos;
    }

} // namespace entity
