#include "generalization.h"
#include "class.h"
#include "enums.h"
#include "helpfunctions.h"
#include "constants.cpp"

#include <QJsonObject>

namespace relationship {

    Generalization::Generalization()
        : Generalization(STUB_ID, STUB_ID, nullptr, nullptr)
    {
    }

    Generalization::Generalization(const QString &tailTypeId, const QString &headTypeId, db::Database *globalDatabase, db::Database *projectDatabase)
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

    QJsonObject Generalization::toJson() const
    {
        auto result = Relation::toJson();

        result.insert("Section", m_Section);

        return result;
    }

    void Generalization::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        Relation::fromJson(src, errorList);

        utility::checkAndSet(src, "Section", errorList, [this, &src](){
            m_Section = static_cast<entity::Section>(src["Section"].toInt());
        });
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
