#pragma once

#include <memory>
#include <map>

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
    enum Section : int;
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

    template <class D>
    std::shared_ptr<entity::Type> findType(const QString &id, const D &database)
    {
        return (database ? database->depthTypeSearch(id) : nullptr);
    }

    template <class D, class... Args>
    std::shared_ptr<entity::Type> findType(const QString &id, const D &database, const Args&... args)
    {
        auto result = findType(id, database);
        if (!result) return findType(id, args...);
        return result;
    }

    template <class D>
    std::shared_ptr<entity::Scope> findScope(const QString &id, const D &database)
    {
        return (database ? database->depthScopeSearch(id) : nullptr);
    }

    template <class D, class... Args>
    std::shared_ptr<entity::Scope> findScope(const QString &id, const D &database, const Args&... args)
    {
        auto result = findScope(id, database);
        if (!result) return findScope(id, args...);
        return result;
    }

    template <class List>
    void deepCopySharedPointerList(const List &src, List &dst)
    {
        List tmpList;
        tmpList.reserve(src.size());

        using ValueType = typename List::value_type::element_type;
        for (auto &&value : src)
            tmpList.append(std::make_shared<ValueType>(*value));

        dst.clear();
        dst = std::move(tmpList);
    }

    template <class List>
    bool listSharedPointerEq(const List &lhs, const List &rhs)
    {
        if (lhs.size() != rhs.size())
            return false;

        using ValueType = typename List::value_type;
        return std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                          [](const ValueType &r, const ValueType &l){ return r == l || *r == *l; });
    }

    template <class Hash, class KeyGetter>
    void deepCopySharedPointerHash(const Hash &src, Hash &dst, KeyGetter keyGetter)
    {
        Hash tmpHash;
        tmpHash.reserve(src.size());

        using ValueType = typename Hash::mapped_type::element_type;
        for (auto &&value : src.values())
            tmpHash.insert(std::bind(keyGetter, value.get())(), std::make_shared<ValueType>(*value));

        dst = std::move(tmpHash);
    }

    template <class Key, class Value>
    Value mapSearchHelper(const std::map<Key, Value> &map, Key key, Value defaultValue)
    {
        Value result = defaultValue;

        auto it(map.find(key));
        if (it != map.end())
            result = it->second;

        return result;
    }

    QString fieldKeywordToString(entity::FieldKeyword keyword);
    QString methodLhsIdToString(entity::LhsIdentificator id);
    QString methodRhsIdToString(entity::RhsIdentificator id);
    QString sectionToString(entity::Section section);
}

