#include "field.h"
#include "extendedtype.h"
#include "enums.h"
#include "constants.cpp"

namespace entity {

    Field::Field()
        : Field(DEFAULT_NAME, STUB_ID)
    {
    }

    Field::Field(const QString &name, const QString &typeId)
        : m_TypeId(typeId)
        , m_Section(Public)
        , m_Name(name)
        , m_Prefix("m_")
    {
    }

    QString Field::name() const
    {
        return m_Name;
    }

    void Field::setName(const QString &name)
    {
        m_Name = name;
    }

    Section Field::section() const
    {
        return m_Section;
    }

    void Field::setSection(Section section)
    {
        m_Section = section;
    }

    QString Field::prefix() const
    {
        return m_Prefix;
    }
    
    void Field::setPrefix(const QString &prefix)
    {
        m_Prefix = prefix;
    }

    FieldKeywordsList Field::keywords() const
    {
        return m_Keywords.values();
    }

    void Field::addKeyword(FieldKeyword keyword)
    {
        m_Keywords << keyword;
    }

    bool Field::containsKeyword(FieldKeyword keyword) const
    {
        return m_Keywords.contains(keyword);
    }

    bool Field::hasKeywords() const
    {
        return m_Keywords.isEmpty();
    }

    void Field::removeKeyword(FieldKeyword keyword)
    {
        m_Keywords.remove(keyword);
    }

    QString Field::typeId() const
    {
        return m_TypeId;
    }

    void Field::setTypeId(const QString &typeId)
    {
        m_TypeId = typeId;
    }

} // namespace entity
