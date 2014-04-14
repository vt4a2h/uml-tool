#include "generalization.h"
#include "class.h"
#include "enums.h"
#include "constants.cpp"

namespace relationship {

    Generalization::Generalization()
        : Generalization(STUB_ID, STUB_ID, nullptr, nullptr)
    {
    }

    Generalization::Generalization(const QString &tailTypeId, const QString &headTypeId, const db::SharedDatabase &globalDatabase, const db::SharedDatabase &projectDatabase)
        :  Relation(tailTypeId, headTypeId, globalDatabase, projectDatabase)
        ,  m_Section(entity::Public)
    {
        m_RelationType = GeneralizationRelation;
    }

    entity::Section Generalization::section() const
    {
        return m_Section;
    }

    void Generalization::setSection(const entity::Section &section)
    {
        m_Section = section;
    }

    void Generalization::make()
    {
        m_TailClass->addParent(m_HeadClass->id(), m_Section);
    }

    void Generalization::clear()
    {
        m_TailClass->removeParent(m_HeadClass->id());
    }

} // namespace relationship
