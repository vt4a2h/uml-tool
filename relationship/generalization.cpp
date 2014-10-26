#include "generalization.h"
#include "enums.h"
#include "constants.cpp"

#include <QJsonObject>

#include <entity/class.h>
#include <utility/helpfunctions.h>

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

    bool operator ==(const Generalization &lhs, const Generalization &rhs)
    {
        return static_cast<const Relation&>(lhs).isEqual(rhs) &&
               lhs.m_Section == rhs.m_Section;
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

    bool Generalization::isEqual(const Generalization &rhs) const
    {
        return *this == rhs;
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
