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

#include "templateclass.h"
#include "constants.cpp"
#include "enums.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

#include <utility/helpfunctions.h>

namespace entity {

    /**
     * @brief TemplateClass::TemplateClass
     */
    TemplateClass::TemplateClass()
        : TemplateClass(DEFAULT_NAME, GLOBAL_SCOPE_ID)
    {
    }

    /**
     * @brief TemplateClass::TemplateClass
     * @param name
     * @param scopeId
     */
    TemplateClass::TemplateClass(const QString &name, const QString &scopeId)
        : Class (name, scopeId)
        , Template()
    {
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const TemplateClass &lhs, const TemplateClass &rhs)
    {
        return static_cast<const Class&>(lhs).isEqual(rhs) &&
               static_cast<const Template&>(lhs).templatePartEq(rhs);
    }

    /**
     * @brief TemplateClass::toJson
     * @return
     */
    QJsonObject TemplateClass::toJson() const
    {
        QJsonObject result(Class::toJson());
        result.insert("Template part", Template::templateToJson());

        return result;
    }

    /**
     * @brief TemplateClass::fromJson
     * @param src
     * @param errorList
     */
    void TemplateClass::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        Class::fromJson(src, errorList);

        utility::checkAndSet(src, "Template part", errorList, [&src, &errorList, this](){
            Template::templateLoadFromJson(src["Template part"].toObject(), errorList);
        });
    }

    /**
     * @brief TemplateClass::hashType
     * @return
     */
    size_t TemplateClass::hashType() const
    {
        return TemplateClass::staticHashType();
    }

    /**
     * @brief TemplateClass::staticHashType
     * @return
     */
    size_t TemplateClass::staticHashType()
    {
        return typeid(TemplateClass).hash_code();
    }

    /**
     * @brief TemplateClass::marker
     * @return
     */
    QString TemplateClass::marker() const
    {
        return TemplateClass::staticMarker();
    }

    /**
     * @brief TemplateClass::staticMarker
     * @return
     */
    QString TemplateClass::staticMarker()
    {
        return "TemplateClass";
    }

    /**
     * @brief TemplateClass::isEqual
     * @param rhs
     * @return
     */
    bool TemplateClass::isEqual(const TemplateClass &rhs) const
    {
        return *this == rhs;
    }

} // namespace entity
