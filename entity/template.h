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
#include "Scope.h"

#include <db/Database.h>
#include <db/db_types.hpp>

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

        TemplateParameter getTemplateParameter(const common::ID &typeId) const;
        TemplateParameter addTemplateParameter(const common::ID &typeId,
                                               const common::ID &defaultTypeId= common::ID::nullID());
        bool contains(const common::ID &typeId) const;
        bool removeParameter(const common::ID &typeId);
        TemplateParametersList templateParameters() const;

        const db::SharedDatabase database() const;
        SharedType getLocalType(const common::ID &typeId) const;
        template <class T = Type> std::shared_ptr<T> addLocalType(const QString &name = "");
        bool containsLocalType(const common::ID &typeId) const;
        void removeLocalType(const common::ID &typeId);
        TypesList localTypes() const;

        QJsonObject templateToJson() const;
        void templateLoadFromJson(const QJsonObject &src, QStringList &errorList);

        bool templatePartEq(const Template &rhs) const;

    private:
        TemplateParametersList m_TemplateParameters;
        db::SharedDatabase m_LocalDatabase;
    };

    template <class T>
    std::shared_ptr<T> Template::addLocalType(const QString &name)
    {
        return m_LocalDatabase->anyScopes() ?
                    m_LocalDatabase->scopes()[0]->addType<T>(name) : nullptr;
    }

} // namespace entity

