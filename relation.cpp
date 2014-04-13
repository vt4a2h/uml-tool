#include "relation.h"
#include "node.h"
#include "class.h"
#include "enums.h"
#include "helpfunctions.h"
#include "constants.cpp"

#include <QJsonObject>

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
        , m_Id(utility::genId())
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

    QString Relation::id() const
    {
        return m_Id;
    }

    void Relation::setId(const QString &id)
    {
        m_Id = id;
    }

    QJsonObject Relation::toJson() const
    {
        QJsonObject result;

        result.insert("ID", m_Id);
        result.insert("Type", m_RelationType);
        result.insert("Description", m_Description);
        result.insert("Head node", m_HeadNode->toJson());
        result.insert("Tail node", m_TailNode->toJson());

        return result;
    }

    void Relation::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        utility::checkAndSet(src, "ID", errorList, [&src, this](){
            m_Id = src["ID"].toString();
        });
        utility::checkAndSet(src, "Description", errorList, [&src, this](){
            m_Description = src["Description"].toString();
        });
        utility::checkAndSet(src, "Type", errorList, [&src, this](){
            m_RelationType = static_cast<RelationType>(src["Type"].toInt());
        });
        utility::checkAndSet(src, "Head node", errorList, [&src, &errorList, this](){
            m_HeadNode->fromJson(src["Head node"].toObject(), errorList);
            // TODO: find head class in db and add it
        });
        utility::checkAndSet(src, "Tail node", errorList, [&src, &errorList, this](){
            m_TailNode->fromJson(src["Tail node"].toObject(), errorList);
            // TODO: find tail class in db and add it
        });
    }

} // namespace relationship
