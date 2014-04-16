#include "relation.h"
#include "node.h"
#include "class.h"
#include "enums.h"
#include "helpfunctions.h"
#include "database.h"
#include "constants.cpp"

#include <QJsonObject>

namespace relationship {

    Relation::Relation()
        : Relation(STUB_ID, STUB_ID, nullptr, nullptr)
    {
    }

    Relation::Relation(const QString &tailTypeId, const QString &headTypeId,
                       const db::SharedDatabase &globalDatabase, const db::SharedDatabase &projectDatabase)
        : m_TailNode(std::make_shared<Node>(tailTypeId))
        , m_HeadNode(std::make_shared<Node>(headTypeId))
        , m_Id(utility::genId())
        , m_RelationType(SimpleRelation)
        , m_GlobalDatabase(globalDatabase)
        , m_ProjectDatabase(projectDatabase)
    {
        Q_ASSERT(addHeadClass(headTypeId));
        Q_ASSERT(addTailClass(tailTypeId));
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
            Q_ASSERT(addHeadClass(m_HeadNode->typeId()));
        });
        utility::checkAndSet(src, "Tail node", errorList, [&src, &errorList, this](){
            m_TailNode->fromJson(src["Tail node"].toObject(), errorList);
            Q_ASSERT(addTailClass(m_TailNode->typeId()));
        });
    }

    db::SharedDatabase Relation::globalDatabase() const
    {
        return m_GlobalDatabase;
    }

    void Relation::setGlobalDatabase(const db::SharedDatabase &globalDatabase)
    {
        m_GlobalDatabase = globalDatabase;
    }

    db::SharedDatabase Relation::projectDatabase() const
    {
        return m_ProjectDatabase;
    }

    void Relation::setProjectDatabase(const db::SharedDatabase &projectDatabase)
    {
        m_ProjectDatabase = projectDatabase;
    }

    bool Relation::addHeadClass(const QString &id)
    {
        m_HeadClass = tryToFindType(id);
        return m_HeadClass.operator bool();
    }

    bool Relation::addTailClass(const QString &id)
    {
        m_TailClass = std::dynamic_pointer_cast<entity::Class>(tryToFindType(id));
        return m_TailClass.operator bool();
    }

    entity::SharedType Relation::tryToFindType(const QString &typeId) const
    {
        entity::SharedType result = m_ProjectDatabase->depthTypeSearch(typeId);
        return (result ? result : m_GlobalDatabase->depthTypeSearch(typeId));
    }



} // namespace relationship
