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
#include "types.h"
#include "scope.h"

#include <db/database.h>

namespace entity {

    // TODO: add existing type posibilities

    /**
     * @brief The Template class
     */
    class Template
    {
    public:
        Template();

        friend bool operator ==(const Template &lhs, const Template &rhs);

        TemplateParameter getTemplateParameter(const QString &typeId) const;
        void addTemplateParameter(const QString &typeId, const QString &defaultTypeId= QString(""));
        bool contains(const QString &typeId) const;
        bool removeParameter(const QString &typeId);
        TemplateParametersList templateParameters() const;

        const db::SharedDatabase database() const;
        SharedType getLocaleType(const QString &typeId) const;
        template <class T = Type> std::shared_ptr<T> addLocaleType(const QString &name = "");
        bool containsLocaleType(const QString &typeId) const;
        void removeLocaleType(const QString &typeId);
        TypesList localeTypes() const;

        QJsonObject templateToJson() const;
        void templateLoadFromJson(const QJsonObject &src, QStringList &errorList);

        bool templatePartEq(const Template &rhs) const;

    private:
        TemplateParametersList m_TemplateParameters;
        db::SharedDatabase m_LocalDatabase;
    };

    template <class T>
    std::shared_ptr<T> Template::addLocaleType(const QString &name)
    {
        return m_LocalDatabase->anyScopes() ?
                    m_LocalDatabase->scopes()[0]->addType<T>(name) : nullptr;
    }

} // namespace entity

