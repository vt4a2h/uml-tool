/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 27/06/2015.
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
#include "property.h"

#include <typeinfo>

#include <QJsonObject>

#include "constants.h"

namespace {
    const QString marker = "property";
}

namespace entity {

    Property::Property()
        : Property(DEFAULT_NAME, STUB_ID, nullptr)
    {
    }

    Property::Property(const QString &name, const QString &id, QObject *parent)
        : BasicEntity(name)
        , m_Id(id)
    {
        setParent(parent);
    }

    QJsonObject Property::toJson() const
    {
        return QJsonObject();
    }

    void Property::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        Q_UNUSED(src); Q_UNUSED(errorList);
    }

    size_t Property::hashType() const
    {
        return Property::staticHashType();
    }

    size_t Property::staticHashType()
    {
        return typeid(Property).hash_code();
    }

    QString Property::marker() const
    {
        return Property::staticMarker();
    }

    QString Property::staticMarker()
    {
        return ::marker;
    }

    QString Property::id() const
    {
        return m_Id;
    }

    void Property::setId(const QString &id)
    {
        m_Id = Id;
    }


} // namespace entity

