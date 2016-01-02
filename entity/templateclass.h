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

#include "class.h"
#include "template.h"

namespace entity {

    /**
     * @brief The TemplateClass class
     */
    class TemplateClass : public Class, public Template
    {
    public:
        TemplateClass();
        TemplateClass(const QString &name, const QString &scopeId);

        friend bool operator ==(const TemplateClass &lhs, const TemplateClass &rhs);

        bool isEqual(const Type &rhs, bool withTypeid = true) const override;

    public: // BasicEntity implementation
        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        size_t hashType() const override;
        static size_t staticHashType();

        QString marker() const override;
        static QString staticMarker();

        QString defaultName() const override;
        static QString staticDefaultName();
    };

} // namespace entity
