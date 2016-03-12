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

#include <common/basicelement.h>
#include <common/meta.h>

#include "entity_types.hpp"
#include "isectional.h"

uint qHash(const entity::SharedField& f);
#include <QSet>

class QJsonObject;
class QStringList;

namespace entity {

    enum Section : int;
    enum FieldKeyword : int;

    /**
     * @brief The Field class
     */
    class Field : public common::BasicElement, public ISectional
    {
    public:
        Field();
        Field(const Field &src);
        Field(Field &&src);
        Field(const QString &name, const EntityID &typeId);
        Field(const QString &name, const EntityID &typeId, const QString &prefix, Section section);

        Field &operator =(Field &&rhs);
        Field &operator =(const Field &rhs);
        friend bool operator== (const Field &lhs, const Field &rhs);

        QString fullName() const;

        QString prefix() const;
        void removePrefix();
        void setPrefix(const QString &prefix);

        FieldKeywordsList keywords() const;
        void addKeyword(FieldKeyword keyword);
        bool containsKeyword(FieldKeyword keyword) const;
        bool hasKeywords() const;
        void removeKeyword(FieldKeyword keyword);

        EntityID typeId() const;
        void setTypeId(const EntityID &typeId);

        QString suffix() const;
        void removeSuffix();
        void setSuffix(const QString &suffix);

        QString defaultValue() const;
        void setDefaultValue(const QString &defaultValue);

    public: // BasicEntity implementation
        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        add_meta(Field)

    public: // ISectional implementation
        Section section() const override;
        void setSection(Section section) override;

    private:
        void copyFrom(const Field &src);
        void moveFrom(Field &&src);

        EntityID m_TypeId;
        Section m_Section;
        QString m_Prefix;
        QString m_Suffix;
        QString m_DefaultValue;

        FieldKeywords m_Keywords;
    };

    inline bool operator==(const WeakField &lhs, const WeakField &rhs)
    {
        return lhs.lock() == rhs.lock();
    }

} // namespace entity
