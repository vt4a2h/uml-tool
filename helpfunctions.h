#pragma once

#include <memory>

class QString;
class QJsonObject;
class QStringList;

namespace std {
    template<class T>
    class function;
}

namespace entity {
    class Type;
    enum UserType : int;

    class ClassMethod;
    enum ClassMethodType : int;

    class Scope;

    enum FieldKeyword : int;
    enum LhsIdentificator : int;
    enum RhsIdentificator : int;
}

namespace relationship {
    class Relation;
    enum RelationType : int;
}

namespace db {
    class Database;
}

namespace utility {

    QString genId();
    void checkAndSet(const QJsonObject& object, const QString& key, QStringList& lst, std::function<void()> func);

    std::shared_ptr<entity::Type> makeType(entity::UserType type);
    std::shared_ptr<relationship::Relation> makeRelation(relationship::RelationType relation);
    std::shared_ptr<entity::ClassMethod> makeMethod(entity::ClassMethodType methodType);

    std::shared_ptr<entity::Type> findType(const std::shared_ptr<db::Database> &global,
                                           const std::shared_ptr<db::Database> & project,
                                           const QString &id);
    std::shared_ptr<entity::Scope> findScope(const std::shared_ptr<db::Database> &global,
                                             const std::shared_ptr<db::Database> & project,
                                             const QString &id);

    QString fieldKeywordToString(entity::FieldKeyword keyword);
    QString methodLhsIdToString(entity::LhsIdentificator id);
    QString methodRhsIdToString(entity::RhsIdentificator id);
}

