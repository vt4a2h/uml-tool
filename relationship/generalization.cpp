#include "generalization.h"
#include "enums.h"
#include "constants.cpp"

#include <QJsonObject>

#include <entity/class.h>
#include <utility/helpfunctions.h>

namespace relationship {

    /**
     * @brief Generalization::Generalization
     */
    Generalization::Generalization()
        : Generalization(STUB_ID, STUB_ID, nullptr, nullptr)
    {
    }

    /**
     * @brief Generalization::Generalization
     * @param tailTypeId
     * @param headTypeId
     * @param globalDatabase
     * @param projectDatabase
     */
    Generalization::Generalization(const QString &tailTypeId, const QString &headTypeId, db::Database *globalDatabase, db::Database *projectDatabase)
        :  Relation(tailTypeId, headTypeId, globalDatabase, projectDatabase)
        ,  m_Section(entity::Public)
    {
        m_RelationType = GeneralizationRelation;
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const Generalization &lhs, const Generalization &rhs)
    {
        return static_cast<const Relation&>(lhs).isEqual(rhs) &&
               lhs.m_Section == rhs.m_Section;
    }

    /**
     * @brief Generalization::section
     * @return
     */
    entity::Section Generalization::section() const
    {
        return m_Section;
    }

    /**
     * @brief Generalization::setSection
     * @param section
     */
    void Generalization::setSection(const entity::Section &section)
    {
        m_Section = section;
    }

    /**
     * @brief Generalization::toJson
     * @return
     */
    QJsonObject Generalization::toJson() const
    {
        auto result = Relation::toJson();

        result.insert("Section", m_Section);

        return result;
    }

    /**
     * @brief Generalization::fromJson
     * @param src
     * @param errorList
     */
    void Generalization::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        Relation::fromJson(src, errorList);

        utility::checkAndSet(src, "Section", errorList, [this, &src](){
            m_Section = static_cast<entity::Section>(src["Section"].toInt());
        });
    }

    /**
     * @brief Generalization::isEqual
     * @param rhs
     * @return
     */
    bool Generalization::isEqual(const Generalization &rhs) const
    {
        return *this == rhs;
    }

    /**
     * @brief Generalization::make
     */
    void Generalization::make()
    {
        m_TailClass->addParent(m_HeadClass->id(), m_Section);
    }

    /**
     * @brief Generalization::clear
     */
    void Generalization::clear()
    {
        m_TailClass->removeParent(m_HeadClass->id());
    }

} // namespace relationship
