#include "node.h"
#include "enums.h"
#include "constants.cpp"

namespace relationship {

    Node::Node()
        :Node(STUB_ID)
    {
    }

    Node::Node(const QString &typeId, Multiplicity multiplicity)
        : m_TypeId(typeId)
        , m_Multiplicity(multiplicity)
    {
    }

    QString Node::typeId() const
    {
        return m_TypeId;
    }

    void Node::setTypeId(const QString &typeId)
    {
        m_TypeId = typeId;
    }

    Multiplicity Node::multiplicity() const
    {
        return m_Multiplicity;
    }

    void Node::setMultiplicity(const Multiplicity &multiplicity)
    {
        m_Multiplicity = multiplicity;
    }



} // namespace relationship
