#include "projectdatabase.h"
#include "relation.h"
#include "helpfunctions.h"

#include <QJsonObject>
#include <QJsonArray>

namespace db {

    ProjectDatabase::ProjectDatabase(ProjectDatabase &&src)
    {
        moveFrom(src);
    }

    ProjectDatabase::ProjectDatabase(const ProjectDatabase &src)
        : Database(src)
    {
        copyFrom(src);
    }

    ProjectDatabase::ProjectDatabase(const QString &name, const QString &path)
        : Database(name, path)
    {
    }

    ProjectDatabase &ProjectDatabase::operator =(ProjectDatabase &&rhs)
    {
        if (this != &rhs)
            moveFrom(rhs);

        return *this;
    }

    ProjectDatabase &ProjectDatabase::operator =(ProjectDatabase rhs)
    {
        moveFrom(rhs);
        return *this;
    }

    relationship::SharedRelation ProjectDatabase::getRelation(const QString &id) const
    {
        return (m_Relations.contains(id) ? m_Relations[id] : nullptr);
    }

    void ProjectDatabase::addRelation(const relationship::SharedRelation &relation)
    {
        m_Relations.insert(relation->id(), relation);
    }

    bool ProjectDatabase::containsRelation(const QString &id) const
    {
        return m_Relations.contains(id);
    }

    void ProjectDatabase::removeRelation(const QString &id)
    {
        m_Relations.remove(id);
    }

    relationship::RelationsList ProjectDatabase::relations() const
    {
        return m_Relations.values();
    }

    void ProjectDatabase::clear()
    {
        Database::clear();
        m_Relations.clear();
    }

    QJsonObject ProjectDatabase::toJson() const
    {
        QJsonObject result(Database::toJson());

        QJsonArray relations;
        for (auto &&val : m_Relations.values()) relations.append(val->toJson());
        result.insert("Relations", relations);

        return result;
    }

    void ProjectDatabase::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        Q_ASSERT_X(m_GlobalDatabase,
                   "ProjectDatabase::fromJson",
                   "global database is not found");
        Database::fromJson(src, errorList);

        utility::checkAndSet(src, "Relations", errorList, [&src, &errorList, this](){
            if (src["Relations"].isArray()) {
                relationship::SharedRelation relation;
                QJsonObject obj;
                for (auto &&val : src["Relations"].toArray()) {
                    obj = val.toObject();
                    utility::checkAndSet(obj, "Type", errorList,
                                         [&obj, &errorList, &relation, this](){
                        relation = utility::makeRelation(static_cast<relationship::RelationType>(obj["Type"].toInt()));
                        relation->setProjectDatabase(this);
                        relation->setGlobalDatabase(m_GlobalDatabase.get());
                        relation->fromJson(obj, errorList);
                        m_Relations.insert(relation->id(), relation);
                    });
                }
            } else {
                errorList << "Error: \"Relations\" is not array";
            }
        });
    }

    void ProjectDatabase::copyFrom(const ProjectDatabase &src)
    {
        m_GlobalDatabase = src.m_GlobalDatabase; // shallow copy. ok
        utility::deepCopySharedPointerHash(src.m_Relations, m_Relations);
    }

    void ProjectDatabase::moveFrom(ProjectDatabase &src)
    {
        Database::moveFrom(src);
        m_GlobalDatabase = std::move(src.m_GlobalDatabase);
        m_Relations = src.m_Relations;
    }

    db::SharedDatabase ProjectDatabase::globalDatabase() const
    {
        return m_GlobalDatabase;
    }

    void ProjectDatabase::setGlobalDatabase(const db::SharedDatabase &globalDatabase)
    {
        Q_ASSERT_X(m_GlobalDatabase,
                   "ProjectDatabase::setGlobalDatabase",
                   "global database is not found");
        m_GlobalDatabase = globalDatabase;
    }


} // namespace db
