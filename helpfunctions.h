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
}

namespace relationship {
    class Relation;
    enum RelationType : int;
}

namespace utility {
    QString genId();
    void checkAndSet(const QJsonObject& object, const QString& key, QStringList& lst, std::function<void()> func);

    std::shared_ptr<entity::Type> makeType(entity::UserType type);
    std::shared_ptr<relationship::Relation> makeRelation(relationship::RelationType relation);
    std::shared_ptr<entity::ClassMethod> makeMethod(entity::ClassMethodType methodType);
}

