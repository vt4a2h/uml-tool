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

#include "TemplateClassMethod.h"
#include "constants.h"
#include "enums.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include <QHash>

#include <utility/helpfunctions.h>

namespace entity {

    /**
     * @brief TemplateClassMethod::TemplateClassMethod
     */
    TemplateClassMethod::TemplateClassMethod()
        : TemplateClassMethod(DEFAULT_NAME)
    {
    }

    /**
     * @brief TemplateClassMethod::TemplateClassMethod
     * @param name
     */
    TemplateClassMethod::TemplateClassMethod(const QString &name)
        : ClassMethod(name)
        , Template()
    {
        m_Type = TemplateMethod;
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const TemplateClassMethod &lhs, const TemplateClassMethod &rhs)
    {
        return static_cast<const ClassMethod&>(lhs).isEqual(rhs) &&
               static_cast<const Template&>(lhs).templatePartEq(rhs);
    }

    /**
     * @brief TemplateClassMethod::toJson
     * @return
     */
    QJsonObject TemplateClassMethod::toJson() const
    {
        QJsonObject result(ClassMethod::toJson());
        result.insert("Template part", Template::templateToJson());

        return result;
    }

    /**
     * @brief TemplateClassMethod::fromJson
     * @param src
     * @param errorList
     */
    void TemplateClassMethod::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        ClassMethod::fromJson(src, errorList);

        utility::checkAndSet(src, "Template part", errorList, [&src, &errorList, this](){
            Template::templateLoadFromJson(src["Template part"].toObject(), errorList);
        });
    }

    /**
     * @brief TemplateClassMethod::isEqual
     * @param rhs
     * @return
     */
    bool TemplateClassMethod::isEqual(const TemplateClassMethod &rhs) const
    {
        return *this == rhs;
    }

} // namespace entity
