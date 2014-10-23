#include "relation.h"
#include "node.h"
#include "class.h"
#include "enums.h"
#include "helpfunctions.h"
#include "database.h"
#include "constants.cpp"

#include <QJsonObject>
#include <QDebug>

namespace relationship {

    Relation::Relation()
        : Relation(STUB_ID, STUB_ID, nullptr, nullptr)
    {
    }

    Relation::Relation(Relation &&src)
    {
        moveFrom(src);
    }

    Relation::Relation(const Relation &src)
    {
        copyFrom(src);
    }

    Relation::Relation(const QString &tailTypeId, const QString &headTypeId,
                       db::Database *globalDatabase, db::Database *projectDatabase)
        : m_TailNode(std::make_shared<Node>(tailTypeId))
        , m_HeadNode(std::make_shared<Node>(headTypeId))
        , m_Id(utility::genId())
        , m_RelationType(SimpleRelation)
        , m_GlobalDatabase(globalDatabase)
        , m_ProjectDatabase(projectDatabase)
    {
        if (headTypeId != STUB_ID)
            addHeadClass(headTypeId);
        if (tailTypeId != STUB_ID)
            addTailClass(tailTypeId);
    }

    Relation::~Relation()
    {
    }

    Relation &Relation::operator =(Relation rhs)
    {
        moveFrom(rhs);

        return *this;
    }

    Relation &Relation::operator =(Relation &&rhs)
    {
        if (this != &rhs)
            moveFrom(rhs);

        return *this;
    }

    bool operator ==(const Relation &lhs, const Relation &rhs)
    {
        return lhs.m_Description     == rhs.m_Description     &&
               lhs.m_GlobalDatabase  == rhs.m_GlobalDatabase  &&
               lhs.m_Id              == rhs.m_Id              &&
               lhs.m_ProjectDatabase == rhs.m_ProjectDatabase &&
               (lhs.m_HeadClass == rhs.m_HeadClass || *lhs.m_HeadClass == *rhs.m_HeadClass) &&
               (lhs.m_HeadNode  == rhs.m_HeadNode  || *lhs.m_HeadNode  == *rhs.m_HeadNode ) &&
               (lhs.m_TailClass == rhs.m_TailClass || *lhs.m_TailClass == *rhs.m_TailClass) &&
               (lhs.m_TailNode  == rhs.m_TailNode  || *lhs.m_TailNode  == *rhs.m_TailNode );
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
        check();
        make();
    }

    void Relation::removeRelation()
    {
        check();
        clear();
    }

    void Relation::make()
    {
    }

    void Relation::clear()
    {
    }

    void Relation::moveFrom(Relation &src)
    {
        m_TailNode = std::move(src.m_TailNode);
        m_HeadNode = std::move(src.m_HeadNode);

        m_HeadClass = std::move(src.m_HeadClass);
        m_TailClass = std::move(src.m_TailClass);

        m_Id = std::move(src.m_Id);
        m_Description = std::move(src.m_Description);
        m_RelationType = std::move(src.m_RelationType);

        m_GlobalDatabase = std::move(src.m_GlobalDatabase);
        m_ProjectDatabase = std::move(src.m_ProjectDatabase);
    }

    void Relation::copyFrom(const Relation &src)
    {
        m_TailNode = std::make_shared<Node>(*src.m_TailNode);
        m_HeadNode = std::make_shared<Node>(*src.m_HeadNode);

        // shallow copy
        m_HeadClass = src.m_HeadClass;
        m_TailClass = src.m_TailClass;

        m_Id = src.m_Id;
        m_Description = src.m_Description;
        m_RelationType = src.m_RelationType;

        m_GlobalDatabase = src.m_GlobalDatabase;
        m_ProjectDatabase = src.m_ProjectDatabase;
    }

    void Relation::check()
    {
        Q_ASSERT_X(m_HeadClass, "Relation::check", "head class not found");
        Q_ASSERT_X(m_TailClass, "Relation::check", "tail class not found");
        Q_ASSERT_X(m_GlobalDatabase, "Relation::check", "glodal database not valid");
        Q_ASSERT_X(m_ProjectDatabase, "Relation::check", "project database not valid");
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

    bool Relation::isEqual(const Relation &rhs) const
    {
        return *this == rhs;
    }

    void Relation::writeToFile(const QString &fileName) const
    {
        utility::writeToFile(*this, fileName);
    }

    bool Relation::readFromFile(const QString &fileName)
    {
        return utility::readFromFile(*this, fileName);
    }

    db::Database *Relation::globalDatabase() const
    {
        return m_GlobalDatabase;
    }

    void Relation::setGlobalDatabase(db::Database *globalDatabase)
    {
        Q_ASSERT_X(globalDatabase,
                   "Relation::setGlobalDatabase",
                   "glodal database not valid");
        m_GlobalDatabase = globalDatabase;
    }

    db::Database *Relation::projectDatabase() const
    {
        return m_ProjectDatabase;
    }

    void Relation::setProjectDatabase(db::Database *projectDatabase)
    {
        Q_ASSERT_X(projectDatabase,
                   "Relation::setProjectDatabase",
                   "project database not valid");
        m_ProjectDatabase = projectDatabase;
    }

    void Relation::addHeadClass(const QString &id)
    {
        auto tmpHead = tryToFindType(id);

        Q_ASSERT_X(tmpHead,
                   "Relation::addHeadClass",
                   "head class not found");

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
