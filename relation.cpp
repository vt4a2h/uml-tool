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
        addHeadClass(headTypeId);
        addTailClass(tailTypeId);
        Q_ASSERT_X(m_GlobalDatabase, "Relation", "glodal database not valid");
        Q_ASSERT_X(m_ProjectDatabase, "Relation", "project database not valid");
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

    void Relation::makeRelation()
    {
        Q_ASSERT_X(m_HeadClass, "Relation::makeRelation", "head class not found");
        Q_ASSERT_X(m_TailClass, "Relation::makeRelation", "tail class not found");
        make();
    }

    void Relation::removeRelation()
    {
        Q_ASSERT_X(m_HeadClass, "Relation::makeRelation", "head class not found");
        Q_ASSERT_X(m_TailClass, "Relation::makeRelation", "tail class not found");
        clear();
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
            addHeadClass(m_HeadNode->typeId());
        });
        utility::checkAndSet(src, "Tail node", errorList, [&src, &errorList, this](){
            m_TailNode->fromJson(src["Tail node"].toObject(), errorList);
            addTailClass(m_TailNode->typeId());
        });
    }

    db::SharedDatabase Relation::globalDatabase() const
    {
        return m_GlobalDatabase;
    }

    void Relation::setGlobalDatabase(const db::SharedDatabase &globalDatabase)
    {
        Q_ASSERT_X(globalDatabase,
                   "Relation::setGlobalDatabase",
                   "glodal database not valid");
        m_GlobalDatabase = globalDatabase;
    }

    db::SharedDatabase Relation::projectDatabase() const
    {
        return m_ProjectDatabase;
    }

    void Relation::setProjectDatabase(const db::SharedDatabase &projectDatabase)
    {
        Q_ASSERT_X(projectDatabase,
                   "Relation::setProjectDatabase",
                   "project database not valid");
        m_ProjectDatabase = projectDatabase;
    }

    void Relation::addHeadClass(const QString &id)
    {
        auto tmpHead = tryToFindType(id);

        Q_ASSERT_X(tmpHead, "Relation::addHeadClass", "head class not found");

        m_HeadClass = tmpHead;
    }

    void Relation::addTailClass(const QString &id)
    {
        auto tmpTailClass = std::dynamic_pointer_cast<entity::Class>(tryToFindType(id));

        Q_ASSERT_X(tmpTailClass,
                   "Relation::addTailClass",
                   "tail class not found or not Class");

        m_TailClass = tmpTailClass ;
    }

    entity::SharedType Relation::tryToFindType(const QString &typeId) const
    {
        entity::SharedType result = m_ProjectDatabase->depthTypeSearch(typeId);
        return (result ? result : m_GlobalDatabase->depthTypeSearch(typeId));
    }

} // namespace relationship
