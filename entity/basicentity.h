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

        virtual QString name() const;
        void setName(const QString &name);

        void writeToFile(const QString &fileName) const;
        bool readFromFile(const QString &fileName);

        virtual QJsonObject toJson() const;
        virtual void fromJson(const QJsonObject &src, QStringList &errorList);

        static constexpr const char *topID = "top_item_id"; // TODO: investigate and eliminate!

        virtual size_t hashType() const;
        static size_t staticHashType();

        virtual QString marker() const;
        static QString staticMarker();

        virtual QString defaultName() const;
        static QString staticDefaultName();

    signals:
        void nameChanged(const QString &newName);

    public:
        BasicEntity &operator =(const BasicEntity &rhs);
        BasicEntity &operator =(BasicEntity &&rhs);

        friend bool operator ==(const BasicEntity &lhs, const BasicEntity &rhs);

    protected:
        BasicEntity() = default;
        explicit BasicEntity(const QString &name);
        BasicEntity(const BasicEntity &src);
        BasicEntity(BasicEntity &&src);
        BasicEntity(const QJsonObject &src, QStringList &errorList);

        QString m_Name;
    };

} // namespace entity
