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
#pragma once

#include "classmethod.h"

#include "basicentity.h"

namespace entity {

    /// The class Property
    class Property : public BasicEntity
    {
    public:
        Property();
        Property(const QString &name, const QString &id, QObject *parent = nullptr);

        Property &addGetter(const QString &customName = "");
        SharedMethod deleteGetter();
        SharedMethod getter() const;

        Property &addSetter(const QString &customName = "");
        SharedMethod &deleteSetter();
        SharedMethod setter();

        Property &addResetter(const QString &customName = "");
        SharedMethod deleteResetter();
        SharedMethod resetter() const;

        Property &addNotifier(const QString &customName = "");
        SharedMethod deleteNotifier();
        SharedMethod notifier() const;

        int revision() const;
        Property &setRevision(int revision);

        bool isDesignable() const;
        Property &setDesignable(bool designable);

        bool isScriptable() const;
        Property &setScriptable() const;

        bool isStored() const;
        Property &setStored(bool stored);

        bool isUser() const;
        Property &setUser(bool user);

        bool isConstant() const;
        Property &setConstant(bool constant);

        bool isFinal() const;
        Property &setFinal(bool final);

    public: // BasicEntity implementation
        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        size_t hashType() const override;
        static size_t staticHashType();

        QString marker() const override;
        static QString staticMarker();

        QString id() const override;
        void setId(const QString &id) override;

    private:
        QString m_Id;
    };

} // namespace entity
