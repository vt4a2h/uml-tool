#include "field.h"
#include "extendedtype.h"

namespace entity {

    Field::Field()
        : Field("noname", nullptr)
    {
    }

    Field::Field(const QString &name, ExtendedType *type)
        : m_Type(type)
        , m_Section(Public)
        , m_Name(name)
    {
    }

    ExtendedType *Field::type() const
    {
        return m_Type;
    }

    void Field::setType(ExtendedType *type)
    {
        m_Type = type;
    }

    QString Field::name() const
    {
        return m_Name;
    }

    void Field::setName(const QString &name)
    {
        m_Name = name;
    }

    Field::Section Field::section() const
    {
        return m_Section;
    }

    void Field::setSection(Field::Section section)
    {
        m_Section = section;
    }

} // namespace entity
