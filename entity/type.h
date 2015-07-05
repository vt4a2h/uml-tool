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

#include "basicentity.h"
#include "icomponents.h"

class QJsonObject;

namespace entity {

    class Scope;

    /**
     * @brief The Type class
     */
    class Type : public BasicEntity, public IComponents
    {
    public:
        Type();
        Type(Type &&src);
        Type(const Type &src);
        Type(const QString &name, const QString &scopeId, const QString &typeId = "");

        Type &operator =(const Type &rhs);
        Type &operator =(Type &&rhs);
        friend bool operator ==(const Type &lhs, const Type &rhs);

        QString scopeId() const;
        void setScopeId(const QString &scopeId); // TODO: perhaps store it as weak pointer

        virtual bool isEqual(const Type &rhs) const;

    public: // BasicEntity implementation
        QString id() const override;
        void setId(const QString &id) override;

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        size_t hashType() const override;
        static size_t staticHashType();

        QString marker() const override;
        static QString staticMarker();

    protected:
        virtual void moveFrom(Type &src);
        virtual void copyFrom(const Type &src);

    private:
        void baseTypeName();

        QString m_Id;
        QString m_ScopeId;

        // position
        QString m_X;
        QString m_Y;
    };

} // namespace entity
