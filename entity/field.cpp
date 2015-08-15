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

#include "field.h"
#include "extendedtype.h"
#include "enums.h"
#include "constants.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

#include <utility/helpfunctions.h>

namespace entity {

    /**
     * @brief Field::Field
     */
    Field::Field()
        : Field(DEFAULT_NAME, STUB_ID)
    {
    }

    /**
     * @brief Field::Field
     * @param src
     */
    Field::Field(const Field &src)
        : BasicEntity( src )
    {
        copyFrom(src);
    }

    /**
     * @brief Field::Field
     * @param src
     */
    Field::Field(Field &&src)
        : BasicEntity(std::forward<Field>(src))
    {
        moveFrom(std::forward<Field>(src));
    }

    /**
     * @brief Field::Field
     * @param name
     * @param typeId
     */
    Field::Field(const QString &name, const QString &typeId)
        : Field(name, typeId, "", Public)
    {
    }

    /**
     * @brief Field::Field
     * @param name
     * @param typeId
     * @param prefix
     * @param section
     */
    Field::Field(const QString &name, const QString &typeId, const QString &prefix, Section section)
        : BasicEntity(name)
        , m_TypeId(typeId)
        , m_Section(section)
        , m_Prefix(prefix)
    {
    }

    /**
     * @brief Field::operator =
     * @param rhs
     * @return
     */
    Field &Field::operator =(Field &&rhs)
    {
        if (this != &rhs) {
            static_cast<BasicEntity*>(this)->operator =(std::forward<Field>(rhs));
            moveFrom(std::forward<Field>(rhs));
        }

        return *this;
    }

    /**
     * @brief Field::operator =
     * @param rhs
     * @return
     */
    Field &Field::operator =(const Field &rhs)
    {
        if (this != &rhs) {
            static_cast<BasicEntity*>(this)->operator =(rhs);
            copyFrom(rhs);
        }

        return *this;
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator==(const Field &lhs, const Field &rhs)
    {
        return static_cast<const BasicEntity&>(lhs) == static_cast<const BasicEntity&>(rhs) &&
               lhs.m_TypeId   == rhs.m_TypeId   &&
               lhs.m_Section  == rhs.m_Section  &&
               lhs.m_Prefix   == rhs.m_Prefix   &&
               lhs.m_Suffix   == rhs.m_Suffix   &&
               lhs.m_Keywords == rhs.m_Keywords &&
               lhs.m_DefaultValue == rhs.m_DefaultValue;
    }

    /**
     * @brief Field::fullName
     * @return
     */
    QString Field::fullName() const
    {
        return QString("%1%2%3").arg(m_Prefix, m_Name, m_Suffix);
    }

    /**
     * @brief Field::section
     * @return
     */
    Section Field::section() const
    {
        return m_Section;
    }

    /**
     * @brief Field::setSection
     * @param section
     */
    void Field::setSection(Section section)
    {
        m_Section = section;
    }

    /**
     * @brief Field::prefix
     * @return
     */
    QString Field::prefix() const
    {
        return m_Prefix;
    }

    /**
     * @brief Field::removePrefix
     */
    void Field::removePrefix()
    {
        m_Prefix.clear();
    }

    /**
     * @brief Field::setPrefix
     * @param prefix
     */
    void Field::setPrefix(const QString &prefix)
    {
        m_Prefix = prefix;
    }

    /**
     * @brief Field::keywords
     * @return
     */
    FieldKeywordsList Field::keywords() const
    {
        return m_Keywords.values();
    }

    /**
     * @brief Field::addKeyword
     * @param keyword
     */
    void Field::addKeyword(FieldKeyword keyword)
    {
        m_Keywords << keyword;
    }

    /**
     * @brief Field::containsKeyword
     * @param keyword
     * @return
     */
    bool Field::containsKeyword(FieldKeyword keyword) const
    {
        return m_Keywords.contains(keyword);
    }

    /**
     * @brief Field::hasKeywords
     * @return
     */
    bool Field::hasKeywords() const
    {
        return m_Keywords.isEmpty();
    }

    /**
     * @brief Field::removeKeyword
     * @param keyword
     */
    void Field::removeKeyword(FieldKeyword keyword)
    {
        m_Keywords.remove(keyword);
    }

    /**
     * @brief Field::typeId
     * @return
     */
    QString Field::typeId() const
    {
        return m_TypeId;
    }

    /**
     * @brief Field::setTypeId
     * @param typeId
     */
    void Field::setTypeId(const QString &typeId)
    {
        m_TypeId = typeId;
    }

    /**
     * @brief Field::toJson
     * @return
     */
    QJsonObject Field::toJson() const
    {
        QJsonObject result = BasicEntity::toJson();

        result.insert("Type ID", m_TypeId);
        result.insert("Section", m_Section);
        result.insert("Prefix", m_Prefix);
        result.insert("Suffix", m_Suffix);
        result.insert("DefaultValue", m_DefaultValue);

        QJsonArray keywords;
        for (auto &&keyword : m_Keywords)
            keywords.append(keyword);
        result.insert("Keywords", keywords);

        return result;
    }

    /**
     * @brief Field::fromJson
     * @param src
     * @param errorList
     */
    void Field::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        BasicEntity::fromJson(src, errorList);

        utility::checkAndSet(src, "Type ID", errorList, [&src, this](){
            m_TypeId = src["Type ID"].toString();
        });
        utility::checkAndSet(src, "Section", errorList, [&src, this](){
            m_Section = static_cast<Section>(src["Section"].toInt());
        });
        utility::checkAndSet(src, "Prefix",  errorList, [&src, this](){
            m_Prefix = src["Prefix"].toString();
        });
        utility::checkAndSet(src, "Suffix",  errorList, [&src, this](){
            m_Suffix = src["Suffix"].toString();
        });
        utility::checkAndSet(src, "DefaultValue",  errorList, [&src, this](){
            m_DefaultValue = src["DefaultValue"].toString();
        });

        m_Keywords.clear();
        utility::checkAndSet(src, "Keywords", errorList, [&src, &errorList, this](){
            if (src["Keywords"].isArray()) {
                for (auto &&value : src["Keywords"].toArray())
                    m_Keywords.insert(static_cast<FieldKeyword>(value.toInt()));
            } else {
                errorList << "Error: \"Keywords\" is not array";
            }
        });
    }

    /**
     * @brief Field::hashType
     * @return
     */
    size_t Field::hashType() const
    {
        return Field::staticHashType();
    }

    /**
     * @brief Field::staticHashType
     * @return
     */
    size_t Field::staticHashType()
    {
        return typeid(Field).hash_code();
    }

    /**
     * @brief Field::copyFrom
     * @param src
     */
    void Field::copyFrom(const Field &src)
    {
        m_TypeId = src.m_TypeId;
        m_Section = src.m_Section;
        m_Prefix = src.m_Prefix;
        m_Suffix = src.m_Suffix;
        m_DefaultValue = src.m_DefaultValue;

        m_Keywords = src.m_Keywords;
    }

    /**
     * @brief Field::moveFrom
     * @param src
     */
    void Field::moveFrom(Field &&src)
    {
        m_TypeId = std::move(src.m_TypeId);
        m_Section = std::move(src.m_Section);
        m_Prefix = std::move(src.m_Prefix);
        m_Suffix = std::move(src.m_Suffix);
        m_DefaultValue = std::move(src.m_DefaultValue);

        m_Keywords = std::move(src.m_Keywords);
    }

    /**
     * @brief Field::suffix
     * @return
     */
    QString Field::suffix() const
    {
        return m_Suffix;
    }

    /**
     * @brief Field::removeSuffix
     */
    void Field::removeSuffix()
    {
        m_Suffix.clear();
    }

    /**
     * @brief Field::setSuffix
     * @param suffix
     */
    void Field::setSuffix(const QString &suffix)
    {
       m_Suffix = suffix;
    }

    /**
     * @brief Field::defaultValue
     * @return
     */
    QString Field::defaultValue() const
    {
        return m_DefaultValue;
    }

    /**
     * @brief Field::setDefaultValue
     * @param defaultValue
     */
    void Field::setDefaultValue(const QString &defaultValue)
    {
        m_DefaultValue = defaultValue;
    }

} // namespace entity
