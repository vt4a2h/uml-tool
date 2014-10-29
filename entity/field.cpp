#include "field.h"
#include "extendedtype.h"
#include "enums.h"
#include "constants.cpp"

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
        : m_TypeId(typeId)
        , m_Section(section)
        , m_Name(name)
        , m_Prefix(prefix)
    {
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator==(const Field &lhs, const Field &rhs)
    {
        return lhs.m_TypeId   == rhs.m_TypeId   &&
               lhs.m_Section  == rhs.m_Section  &&
               lhs.m_Name     == rhs.m_Name     &&
               lhs.m_Prefix   == rhs.m_Prefix   &&
               lhs.m_Suffix   == rhs.m_Suffix   &&
               lhs.m_Keywords == rhs.m_Keywords &&
               lhs.m_DefaultValue == rhs.m_DefaultValue;
    }

    /**
     * @brief Field::name
     * @return
     */
    QString Field::name() const
    {
        return m_Name;
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
     * @brief Field::setName
     * @param name
     */
    void Field::setName(const QString &name)
    {
        m_Name = name;
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
        QJsonObject result;

        result.insert("Type ID", m_TypeId);
        result.insert("Section", m_Section);
        result.insert("Name", m_Name);
        result.insert("Prefix", m_Prefix);
        result.insert("Suffix", m_Suffix);
        result.insert("DefaultValue", m_DefaultValue);

        QJsonArray keywords;
        for (auto keyword : m_Keywords) keywords.append(keyword);
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
        utility::checkAndSet(src, "Type ID", errorList, [&src, this](){
            m_TypeId = src["Type ID"].toString();
        });
        utility::checkAndSet(src, "Section", errorList, [&src, this](){
            m_Section = static_cast<Section>(src["Section"].toInt());
        });
        utility::checkAndSet(src, "Name",    errorList, [&src, this](){
            m_Name = src["Name"].toString();
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

    /**
     * @brief Field::clone
     * @return
     */
    Field *Field::clone() const
    {
        return new Field(*this);
    }

    /**
     * @brief Field::writeToFile
     * @param fileName
     */
    void Field::writeToFile(const QString &fileName) const
    {
        utility::writeToFile(*this, fileName);
    }

    /**
     * @brief Field::readFromFile
     * @param fileName
     * @return
     */
    bool Field::readFromFile(const QString &fileName)
    {
        return utility::readFromFile(*this, fileName);
    }

} // namespace entity
