#include "node.h"
#include "enums.h"
#include "constants.cpp"

#include <QJsonObject>

#include <utility/helpfunctions.h>

namespace relationship {

    /**
     * @brief Node::Node
     */
    Node::Node()
        :Node(STUB_ID)
    {
    }

    /**
     * @brief Node::Node
     * @param typeId
     * @param multiplicity
     */
    Node::Node(const QString &typeId, Multiplicity multiplicity)
        : m_TypeId(typeId)
        , m_Description(DEFAULT_DESCRIPTION)
        , m_Multiplicity(multiplicity)
    {
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const Node &lhs, const Node &rhs)
    {
        return lhs.m_TypeId       == rhs.m_TypeId      &&
               lhs.m_Description  == rhs.m_Description &&
               lhs.m_Multiplicity == rhs.m_Multiplicity;
    }

    /**
     * @brief Node::typeId
     * @return
     */
    QString Node::typeId() const
    {
        return m_TypeId;
    }

    /**
     * @brief Node::setTypeId
     * @param typeId
     */
    void Node::setTypeId(const QString &typeId)
    {
        m_TypeId = typeId;
    }

    /**
     * @brief Node::toJson
     * @return
     */
    QJsonObject Node::toJson() const
    {
        QJsonObject result;

        result.insert("Type ID", m_TypeId);
        result.insert("Description", m_Description);
        result.insert("Multiplicity", m_Multiplicity);

        return result;
    }

    /**
     * @brief Node::fromJson
     * @param src
     * @param errorList
     */
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

    /**
     * @brief Node::writeToFile
     * @param fileName
     */
    void Node::writeToFile(const QString &fileName) const
    {
        utility::writeToFile(*this, fileName);
    }

    /**
     * @brief Node::readFromFile
     * @param fileName
     * @return
     */
    bool Node::readFromFile(const QString &fileName)
    {
       return utility::readFromFile(*this, fileName);
    }

    /**
     * @brief Node::multiplicity
     * @return
     */
    Multiplicity Node::multiplicity() const
    {
        return m_Multiplicity;
    }

    /**
     * @brief Node::setMultiplicity
     * @param multiplicity
     */
    void Node::setMultiplicity(const Multiplicity &multiplicity)
    {
        m_Multiplicity = multiplicity;
    }

    /**
     * @brief Node::description
     * @return
     */
    QString Node::description() const
    {
        return m_Description;
    }

    /**
     * @brief Node::setDescription
     * @param description
     */
    void Node::setDescription(const QString &description)
    {
        m_Description = description;
    }

} // namespace relationship
