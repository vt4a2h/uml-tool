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

class QJsonObject;

namespace entity {

    class Scope;
    enum UserType : int;

    /**
     * @brief The Type class
     */
    class Type
    {
    public:
        Type();
        Type(Type &&src);
        Type(const Type &src);
        Type(const QString &name, const QString &scopeId, const QString &typeId = "");
        virtual ~Type();

        Type &operator =(Type rhs);
        Type &operator =(Type &&rhs);
        friend bool operator ==(const Type &lhs, const Type &rhs);

        QString name() const;
        void setName(const QString &name);

        QString id() const;
        void setId(const QString &id);

        QString scopeId() const;
        void setScopeId(const QString &scopeId);

        UserType type() const;

        virtual QJsonObject toJson() const;
        virtual void fromJson(const QJsonObject &src, QStringList &errorList);

        void writeToFile(const QString &fileName) const;
        bool readFromFile(const QString &fileName);

        virtual Type *clone() const;

        virtual bool isEqual(const Type &rhs) const;

    protected:
        virtual void moveFrom(Type &src);
        virtual void copyFrom(const Type &src);

        UserType m_KindOfType;

    private:
        void generateUniqueName();

        QString m_Name;
        QString m_Id;
        QString m_ScopeId;

        // position
        QString m_X;
        QString m_Y;
    };

} // namespace entity
