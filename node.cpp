#include "node.h"
#include "enums.h"
#include "helpfunctions.h"
#include "constants.cpp"

#include <QJsonObject>

namespace relationship {

    Node::Node()
        :Node(STUB_ID)
    {
    }

    Node::Node(const QString &typeId, Multiplicity multiplicity)
        : m_TypeId(typeId)
        , m_Description(DEFAULT_DESCRIPTION)
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

    QJsonObject Node::toJson() const
    {
        QJsonObject result;

        result.insert("Type ID", m_TypeId);
        result.insert("Description", m_Description);
        result.insert("Multiplicity", m_Multiplicity);

        return result;
    }

    void Node::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        utility::checkAndSet(src, "Type ID", errorList, [&src, this](){
            m_TypeId = src["Type ID"].toString();
        });
        utility::checkAndSet(src, "Description", errorList, [&src, this](){
            m_Description = src["Description"].toString();
        });
        utility::checkAndSet(src, "Multiplicity", errorList, [&src, this](){
            m_Multiplicity = static_cast<Multiplicity>(src["Multiplicity"].toInt());
        });
    }

    Multiplicity Node::multiplicity() const
    {
        return m_Multiplicity;
    }

    void Node::setMultiplicity(const Multiplicity &multiplicity)
    {
        m_Multiplicity = multiplicity;
    }

    QString Node::description() const
    {
        return m_Description;
    }

    void Node::setDescription(const QString &description)
    {
        m_Description = description;
    }

} // namespace relationship
