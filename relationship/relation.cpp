#include "relation.h"
#include "node.h"
#include "enums.h"
#include "constants.cpp"

#include <QJsonObject>
#include <QDebug>

#include <db/database.h>
#include <entity/class.h>
#include <utility/helpfunctions.h>

namespace relationship {

    /**
     * @brief Relation::Relation
     */
    Relation::Relation()
        : Relation(STUB_ID, STUB_ID, nullptr, nullptr)
    {
    }

    /**
     * @brief Relation::Relation
     * @param src
     */
    Relation::Relation(Relation &&src)
    {
        moveFrom(src);
    }

    /**
     * @brief Relation::Relation
     * @param src
     */
    Relation::Relation(const Relation &src)
    {
        copyFrom(src);
    }

    /**
     * @brief Relation::Relation
     * @param tailTypeId
     * @param headTypeId
     * @param globalDatabase
     * @param projectDatabase
     */
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

    /**
     * @brief Relation::~Relation
     */
    Relation::~Relation()
    {
    }

    /**
     * @brief Relation::operator =
     * @param rhs
     * @return
     */
    Relation &Relation::operator =(Relation rhs)
    {
        moveFrom(rhs);

        return *this;
    }

    /**
     * @brief Relation::operator =
     * @param rhs
     * @return
     */
    Relation &Relation::operator =(Relation &&rhs)
    {
        if (this != &rhs)
            moveFrom(rhs);

        return *this;
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
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

    /**
     * @brief Relation::description
     * @return
     */
    QString Relation::description() const
    {
        return m_Description;
    }

    /**
     * @brief Relation::setDescription
     * @param description
     */
    void Relation::setDescription(const QString &description)
    {
        m_Description = description;
    }

    /**
     * @brief Relation::makeRelation
     */
    void Relation::makeRelation()
    {
        check();
        make();
    }

    /**
     * @brief Relation::removeRelation
     */
    void Relation::removeRelation()
    {
        check();
        clear();
    }

    /**
     * @brief Relation::make
     */
    void Relation::make()
    {
    }

    /**
     * @brief Relation::clear
     */
    void Relation::clear()
    {
    }

    /**
     * @brief Relation::moveFrom
     * @param src
     */
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

    /**
     * @brief Relation::copyFrom
     * @param src
     */
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

    /**
     * @brief Relation::check
     */
    void Relation::check()
    {
        Q_ASSERT_X(m_HeadClass, "Relation::check", "head class not found");
        Q_ASSERT_X(m_TailClass, "Relation::check", "tail class not found");
        Q_ASSERT_X(m_GlobalDatabase, "Relation::check", "glodal database not valid");
        Q_ASSERT_X(m_ProjectDatabase, "Relation::check", "project database not valid");
    }

    /**
     * @brief Relation::relationType
     * @return
     */
    RelationType Relation::relationType() const
    {
        return m_RelationType;
    }

    /**
     * @brief Relation::setRelationType
     * @param relationType
     */
    void Relation::setRelationType(const RelationType &relationType)
    {
        m_RelationType = relationType;
    }

    /**
     * @brief Relation::id
     * @return
     */
    QString Relation::id() const
    {
        return m_Id;
    }

    /**
     * @brief Relation::setId
     * @param id
     */
    void Relation::setId(const QString &id)
    {
        m_Id = id;
    }

    /**
     * @brief Relation::toJson
     * @return
     */
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

    /**
     * @brief Relation::fromJson
     * @param src
     * @param errorList
     */
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

    /**
     * @brief Relation::isEqual
     * @param rhs
     * @return
     */
    bool Relation::isEqual(const Relation &rhs) const
    {
        return *this == rhs;
    }

    /**
     * @brief Relation::writeToFile
     * @param fileName
     */
    void Relation::writeToFile(const QString &fileName) const
    {
        utility::writeToFile(*this, fileName);
    }

    /**
     * @brief Relation::readFromFile
     * @param fileName
     * @return
     */
    bool Relation::readFromFile(const QString &fileName)
    {
        return utility::readFromFile(*this, fileName);
    }

    /**
     * @brief Relation::globalDatabase
     * @return
     */
    db::Database *Relation::globalDatabase() const
    {
        return m_GlobalDatabase;
    }

    /**
     * @brief Relation::setGlobalDatabase
     * @param globalDatabase
     */
    void Relation::setGlobalDatabase(db::Database *globalDatabase)
    {
        Q_ASSERT_X(globalDatabase,
                   "Relation::setGlobalDatabase",
                   "glodal database not valid");
        m_GlobalDatabase = globalDatabase;
    }

    /**
     * @brief Relation::projectDatabase
     * @return
     */
    db::Database *Relation::projectDatabase() const
    {
        return m_ProjectDatabase;
    }

    /**
     * @brief Relation::setProjectDatabase
     * @param projectDatabase
     */
    void Relation::setProjectDatabase(db::Database *projectDatabase)
    {
        Q_ASSERT_X(projectDatabase,
                   "Relation::setProjectDatabase",
                   "project database not valid");
        m_ProjectDatabase = projectDatabase;
    }

    /**
     * @brief Relation::addHeadClass
     * @param id
     */
    void Relation::addHeadClass(const QString &id)
    {
        auto tmpHead = tryToFindType(id);

        Q_ASSERT_X(tmpHead,
                   "Relation::addHeadClass",
                   "head class not found");

        m_HeadClass = tmpHead;
    }

    /**
     * @brief Relation::addTailClass
     * @param id
     */
    void Relation::addTailClass(const QString &id)
    {
        auto tmpTailClass = std::dynamic_pointer_cast<entity::Class>(tryToFindType(id));

        Q_ASSERT_X(tmpTailClass,
                   "Relation::addTailClass",
                   "tail class not found or not Class");

        m_TailClass = tmpTailClass ;
    }

    /**
     * @brief Relation::tryToFindType
     * @param typeId
     * @return
     */
    entity::SharedType Relation::tryToFindType(const QString &typeId) const
    {
        entity::SharedType result = m_ProjectDatabase->depthTypeSearch(typeId);
        return (result ? result : m_GlobalDatabase->depthTypeSearch(typeId));
    }

} // namespace relationship
