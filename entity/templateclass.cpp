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

#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

#include <utility/helpfunctions.h>

#include "constants.h"
#include "enums.h"
#include "entityid.h"

namespace entity {

    /**
     * @brief TemplateClass::TemplateClass
     */
    TemplateClass::TemplateClass()
        : TemplateClass("Template class", EntityID::globalScopeID())
    {
    }

    /**
     * @brief TemplateClass::TemplateClass
     * @param name
     * @param scopeId
     */
    TemplateClass::TemplateClass(const QString &name, const EntityID &scopeId)
        : Class (name, scopeId)
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
        return lhs.isEqual(rhs);
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
     * @brief TemplateClass::defaultName
     * @return
     */
    QString TemplateClass::defaultName() const
    {
        return staticDefaultName();
    }

    /**
     * @brief TemplateClass::staticDefaultName
     * @return
     */
    QString TemplateClass::staticDefaultName()
    {
       return tr("Template class");
    }

    /**
     * @brief TemplateClass::isEqual
     * @param rhs
     * @return
     */
    bool TemplateClass::isEqual(const Type &rhs, bool withTypeid) const
    {
        return Class::isEqual(rhs, withTypeid) && templatePartEq(static_cast<const TemplateClass &>(rhs));
    }

} // namespace entity
