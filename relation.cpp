#include "relation.h"
#include "node.h"
#include "class.h"
#include "enums.h"
#include "constants.cpp"

namespace relationship {

    Relation::Relation()
        : Relation(STUB_ID, STUB_ID)
    {
    }

    Relation::Relation(const QString &tailTypeId, const QString &headTypeId)
        : m_TailNode(std::make_shared<Node>(tailTypeId))
        , m_HeadNode(std::make_shared<Node>(headTypeId))
        , m_HeadClass(std::make_shared<entity::Class>()) // NOTE: stub. load class from db
        , m_TailClass(std::make_shared<entity::Class>()) // NOTE: stub. load class from db
        , m_RelationType(SimpleRelation)
    {
    }

    Relation::~Relation()
    {
    }

    QString Relation::description() const
    {
        return m_Description;
    }

    void Relation::setDescription(const QString &description)
    {
        m_Description = description;
    }

    void Relation::make()
    {
    }

    void Relation::clear()
    {
    }

    RelationType Relation::relationType() const
    {
        return m_RelationType;
    }

    void Relation::setRelationType(const RelationType &relationType)
    {
        m_RelationType = relationType;
    }


} // namespace relationship
