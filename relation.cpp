#include "relation.h"
#include "node.h"
#include "constants.cpp"

namespace relationship {

    Relation::Relation()
        : Relation(STUB_ID, STUB_ID)
    {
    }

    Relation::Relation(const QString &tailTypeId, const QString &headTypeId)
        : m_TailNode(std::make_shared<Node>(tailTypeId))
        , m_HeadNode(std::make_shared<Node>(headTypeId))
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


} // namespace relationship
