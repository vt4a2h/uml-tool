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

#include "classmethod.h"
#include "template.h"

namespace entity {

    /**
     * @brief The TemplateClassMethod class
     */
    class TemplateClassMethod : public ClassMethod, public Template
    {
    public:
        TemplateClassMethod();
        TemplateClassMethod(const QString &name);

        friend bool operator ==(const TemplateClassMethod &lhs, const TemplateClassMethod &rhs);

        bool isEqual(const TemplateClassMethod &rhs) const;

    public: // BasicEntity implementation
        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        add_meta(TemplateClassMethod)
    };

} // namespace entity
