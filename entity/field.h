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

#include "types.h"

#include <QString>

class QJsonObject;
class QStringList;

namespace entity {

    enum Section : int;
    enum FieldKeyword : int;

    /**
     * @brief The Field class
     */
    class Field
    {
    public:
        Field();
        Field(const QString &name, const QString &typeId);
        Field(const QString &name, const QString &typeId, const QString &prefix, Section section);

        friend bool operator== (const Field &lhs, const Field &rhs);

        QString name() const;
        QString fullName() const;
        void setName(const QString &name);

        Section section() const;
        void setSection(Section section);

        QString prefix() const;
        void removePrefix();
        void setPrefix(const QString &prefix);

        FieldKeywordsList keywords() const;
        void addKeyword(FieldKeyword keyword);
        bool containsKeyword(FieldKeyword keyword) const;
        bool hasKeywords() const;
        void removeKeyword(FieldKeyword keyword);

        QString typeId() const;
        void setTypeId(const QString &typeId);

        QJsonObject toJson() const;
        void fromJson(const QJsonObject &src, QStringList &errorList);

        QString suffix() const;
        void removeSuffix();
        void setSuffix(const QString &suffix);

        QString defaultValue() const;
        void setDefaultValue(const QString &defaultValue);

        virtual Field *clone() const;

        void writeToFile(const QString &fileName) const;
        bool readFromFile(const QString &fileName);

        QString id() const;

    private:
        QString m_TypeId;
        Section m_Section;
        QString m_Name;
        QString m_Prefix;
        QString m_Suffix;
        QString m_DefaultValue;

        FieldKeywords m_Keywords;
    };

} // namespace entity
