/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 09/02/2015.
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

#include <QObject>

namespace entity {

    /// The BasicEntity class
    class BasicEntity : public QObject
    {
        Q_OBJECT

    public:
        virtual QString id() const;
        virtual QString parentID() const; // TODO: implement! it's usefull
        virtual void setId(const QString &id);

        void writeToFile(const QString &fileName) const;
        bool readFromFile(const QString &fileName);

        virtual QJsonObject toJson() const = 0;
        virtual void fromJson(const QJsonObject &src, QStringList &errorList) = 0;

        static constexpr const char *topID = "top_item_id";
    };

} // namespace entity
