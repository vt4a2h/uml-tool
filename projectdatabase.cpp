#include "projectdatabase.h"
#include "relation.h"
#include "helpfunctions.h"

#include <QJsonObject>
#include <QJsonArray>

namespace db {

    ProjectDatabase::ProjectDatabase(const QString &name, const QString &path)
        : Database(name, path)
    {
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
        for (auto val : m_Relations.values()) relations.append(val->toJson());
        result.insert("Relations", relations);

        return result;
    }

    void ProjectDatabase::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        Database::fromJson(src, errorList);

        utility::checkAndSet(src, "Relations", errorList, [&src, &errorList, this](){
            if (src["Relations"].isArray()) {
                relationship::SharedRelation relation;
                QJsonObject obj;
                for (auto val : src["Relations"].toArray()) {
                    obj = val.toObject();
                    utility::checkAndSet(obj, "Type", errorList,
                                         [&obj, &errorList, &relation, this](){
                        relation = utility::makeRelation(static_cast<relationship::RelationType>(obj["Type"].toInt()));
                        relation->fromJson(obj, errorList);
                        m_Relations.insert(relation->id(), relation);
                    });
                }
            } else {
                errorList << "Error: \"Relations\" is not array";
            }
        });
    }

} // namespace db
