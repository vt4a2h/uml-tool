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
#include "extendedtype.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

#include <boost/range/algorithm/find_if.hpp>

#include <utility/helpfunctions.h>

#include "enums.h"
#include "constants.h"

using namespace boost;

namespace entity {

    /**
     * @brief ExtendedType::ExtendedType
     */
    ExtendedType::ExtendedType()
        : ExtendedType(defaultName(), common::ID::nullID())
    {
    }

    /**
     * @brief ExtendedType::ExtendedType
     * @param name
     * @param scopeId
     */
    ExtendedType::ExtendedType(const QString &name, const common::ID &scopeId)
        : Type(name, scopeId)
        , m_ConstStatus(false)
        , m_UseAlias(false)
    {
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const ExtendedType &lhs, const ExtendedType &rhs)
    {
        return lhs.isEqual(rhs);
    }

    /**
     * @brief ExtendedType::isLink
     * @return
     */
    bool ExtendedType::isLink() const
    {
        return range::find_if(m_PointersAndLinks, [](auto &&pl){ return pl.first == "&"; }) !=
               m_PointersAndLinks.end();
    }

    /**
     * @brief ExtendedType::addPointerStatus
     * @param pointerToConst
     */
    void ExtendedType::addPointerStatus(bool pointerToConst)
    {
        m_PointersAndLinks.append({"*", pointerToConst});
    }

    /**
     * @brief ExtendedType::removePointerStatus
     */
    void ExtendedType::removePointerStatus()
    {
        if (!m_PointersAndLinks.isEmpty() && m_PointersAndLinks.last().first == "*")
            m_PointersAndLinks.removeLast();
    }

    /**
     * @brief ExtendedType::isPointer
     * @return
     */
    bool ExtendedType::isPointer() const
    {
        return range::find_if(m_PointersAndLinks, [](auto &&pl){ return pl.first == "*"; }) !=
               m_PointersAndLinks.end();
    }

    /**
     * @brief ExtendedType::addLinkStatus
     */
    void ExtendedType::addLinkStatus()
    {
        m_PointersAndLinks.append({"&", false});
    }

    /**
     * @brief ExtendedType::removeLinkStatus
     */
    void ExtendedType::removeLinkStatus()
    {
        if (!m_PointersAndLinks.isEmpty() && m_PointersAndLinks.last().first == "&")
            m_PointersAndLinks.removeLast();
    }

    /**
     * @brief ExtendedType::pl
     * @return
     */
    ExtendedType::PlList ExtendedType::pl() const
    {
        return m_PointersAndLinks;
    }

    /**
     * @brief ExtendedType::isConst
     * @return
     */
    bool ExtendedType::isConst() const
    {
        return m_ConstStatus;
    }

    /**
     * @brief ExtendedType::setConstStatus
     * @param status
     */
    void ExtendedType::setConstStatus(bool status)
    {
        m_ConstStatus = status;
    }

    /**
     * @brief ExtendedType::addTemplateParameter
     * @param typeId
     */
    void ExtendedType::addTemplateParameter(const common::ID &typeId)
    {
        m_TemplateParameters << typeId;
    }

    /**
     * @brief ExtendedType::containsTemplateParameter
     * @param typeId
     * @return
     */
    bool ExtendedType::containsTemplateParameter(const common::ID &typeId) const
    {
        return m_TemplateParameters.contains(typeId);
    }

    /**
     * @brief ExtendedType::removeTemplateParameters
     * @param typeId
     */
    void ExtendedType::removeTemplateParameters(const common::ID &typeId)
    {
        m_TemplateParameters.remove(m_TemplateParameters.indexOf(typeId));
    }

    /**
     * @brief ExtendedType::templateParameters
     * @return
     */
    ExtendedTypesIdList ExtendedType::templateParameters() const
    {
        return m_TemplateParameters;
    }

    /**
     * @brief ExtendedType::typeId
     * @return
     */
    common::ID ExtendedType::typeId() const
    {
        return m_TypeId;
    }

    /**
     * @brief ExtendedType::setTypeId
     * @param typeId
     */
    void ExtendedType::setTypeId(const common::ID &typeId)
    {
        m_TypeId = typeId;
    }

    /**
     * @brief ExtendedType::toJson
     * @return
     */
    QJsonObject ExtendedType::toJson() const
    {
        QJsonObject result(Type::toJson());

        result.insert("Const status", m_ConstStatus);
        result.insert("Type id", m_TypeId.toJson());
        result.insert("Use alias", m_UseAlias);

        QJsonArray pointersAndLinks;
        QJsonObject obj;
        for (auto &&p : m_PointersAndLinks) {
            obj.insert("Pl", p.first);
            obj.insert("Const pl status", p.second);
            pointersAndLinks.append(obj);
        }
        result.insert("Pointers and links", pointersAndLinks);

        QJsonArray templateParameters;
        for (auto &&parameterId : m_TemplateParameters)
            templateParameters.append(parameterId.toJson());
        result.insert("Template parameters", templateParameters);

        return result;
    }

    /**
     * @brief ExtendedType::fromJson
     * @param src
     * @param errorList
     */
    void ExtendedType::fromJson(const QJsonObject &src, QStringList &errorList)
    {
       Type::fromJson(src, errorList);
       utility::checkAndSet(src, "Const status", errorList,
                            [&src, this](){ m_ConstStatus = src["Const status"].toBool(); });
       utility::checkAndSet(src, "Type id", errorList,
                            [&src, &errorList, this](){ m_TypeId.fromJson(src["Type id"], errorList); });
       utility::checkAndSet(src, "Use alias", errorList,
                            [&src, this](){ m_UseAlias = src["Use alias"].toBool(); });

       m_PointersAndLinks.clear();
       utility::checkAndSet(src, "Pointers and links", errorList, [&src, &errorList, this](){
           if (src["Pointers and links"].isArray()) {
               Pl pl;
               QJsonObject obj;
               for (auto &&value : src["Pointers and links"].toArray()) {
                    obj = value.toObject();
                    utility::checkAndSet(obj, "Pl", errorList,
                                         [&obj, &pl](){ pl.first = obj["Pl"].toString(); });
                    utility::checkAndSet(obj, "Const pl status", errorList,
                                         [&obj, &pl](){ pl.second = obj["Const pl status"].toBool(); });
                    m_PointersAndLinks << pl;
               }
           } else {
               errorList << "Error: \"Pointers and links\" is not array";
           }
       });

       m_TemplateParameters.clear();
       utility::checkAndSet(src, "Template parameters", errorList, [&src, &errorList, this](){
           if (src["Template parameters"].isArray()) {
               for (auto &&value : src["Template parameters"].toArray()) {
                   common::ID tmp;
                   tmp.fromJson(value, errorList);
                   m_TemplateParameters << tmp;
               }
           } else {
               errorList << "Error: \"Template parameters\" is not array";
           }
       });
    }

    /**
     * @brief ExtendedType::defaultName
     * @return
     */
    QString ExtendedType::defaultName() const
    {
        return staticDefaultName();
    }

    /**
     * @brief ExtendedType::staticDefaultName
     * @return
     */
    QString ExtendedType::staticDefaultName()
    {
       return tr("Alias");
    }

    /**
     * @brief ExtendedType::useAlias
     * @return
     */
    bool ExtendedType::useAlias() const
    {
        return m_UseAlias;
    }

    /**
     * @brief ExtendedType::setUseAlias
     * @param useAlias
     */
    void ExtendedType::setUseAlias(bool useAlias)
    {
        m_UseAlias = useAlias;
    }

    /**
     * @brief ExtendedType::isEqual
     * @param rhs
     * @return
     */
    bool ExtendedType::isEqual(const Type &rhs, bool withTypeid) const
    {
        if (rhs.hashType() != this->hashType())
            return false;

        auto r = static_cast<const ExtendedType &>(rhs);
        return Type::isEqual(r, withTypeid)                   &&
               m_ConstStatus        == r.m_ConstStatus        &&
               m_TypeId             == r.m_TypeId             &&
               m_PointersAndLinks   == r.m_PointersAndLinks   &&
               m_TemplateParameters == r.m_TemplateParameters &&
               m_UseAlias           == r.m_UseAlias;
    }

} // namespace entity
