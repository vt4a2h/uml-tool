/*****************************************************************************
**
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 03/11/2014.
**
** This file is part of Q-UML (UML tool for Qt).
**
** Q-UML is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Q-UML is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.

** You should have received a copy of the GNU Lesser General Public License
** along with Q-UML.  If not, see <http://www.gnu.org/licenses/>.
**
*****************************************************************************/

#pragma once

#include <memory>
#include <map>
#include <functional>

#include <range/v3/algorithm/equal.hpp>
#include <range/v3/algorithm/any_of.hpp>

#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>

#include <DB/DBTypes.hpp>
#include <DB/Database.h>
#include <Entity/EntityTypes.hpp>

#include "types.h"

class QString;
class QJsonObject;
class QStringList;

namespace Entity {
    class Type;
    enum UserType : int;

    class ClassMethod;
    enum ClassMethodType : int;

    class Scope;

    enum FieldKeyword : int;
    enum class LhsIdentificator : int;
    enum class RhsIdentificator : int;
    enum Section : int;
}

namespace Relationship {
    class Relation;
    enum RelationType : int;
}

namespace DB {
    class Database;
}

namespace Util {

    // TODO: move something to helpers.h / cpp

    QString genId();
    void checkAndSet(const QJsonObject& object, const QString& key, QStringList& lst, const std::function<void()>& func);

    std::shared_ptr<Entity::Type> makeType(const QString &hash);
    std::shared_ptr<Entity::ClassMethod> makeMethod(Entity::ClassMethodType methodType);

    QStringList scopesNamesList(const Entity::SharedType &type, const DB::SharedDatabase &DB);

    template <class... Args>
    std::shared_ptr<Entity::Type> findType(const Common::ID &id, const Args&... args)
    {
        for (auto &&db: std::vector<DB::SharedDatabase>{args...})
            if (db)
                if (auto type = db->typeByID(id))
                    return type;

        return nullptr;
    }

    template <class... Args>
    std::shared_ptr<Entity::Scope> findScope(const Common::ID &id, const Args&... args)
    {
        for (auto &&db: std::vector<DB::SharedDatabase>{args...})
            if (db)
                if (auto scope = db->scope(id, true /*searchInDepth*/))
                    return scope;

        return nullptr;
    }

    template <class List>
    void deepCopyPointerList(const List &src, List &dst)
    {
        List tmpList;
        tmpList.reserve(src.size());

        using ValueType = typename std::remove_pointer<typename List::value_type>::type;
        for (auto &&value : src)
            tmpList.append(new ValueType(*value));

        dst = std::move(tmpList);
    }

    template <class List>
    void deepCopySharedPointerList(const List &src, List &dst)
    {
        List tmpList;
        tmpList.reserve(src.size());

        using ValueType = typename List::value_type::element_type;
        for (auto &&value : src)
            tmpList.append(std::make_shared<ValueType>(*value));

        dst = std::move(tmpList);
    }

    template<class T>
    inline bool sharedPtrEq(const std::shared_ptr<T> &lhs, const std::shared_ptr<T> &rhs)
    {
        // The same pointers
        if (lhs == rhs || (!lhs && !rhs))
            return true;

        if (lhs && rhs)
            return *lhs == *rhs;

        return false;
    }

    template <class Container>
    bool seqSharedPointerEq(const Container &lhs, const Container &rhs)
    {
        if (lhs.size() != rhs.size())
            return false;

        return ranges::equal(lhs, rhs, [](auto &&r, auto &&l){ return sharedPtrEq(l, r); });
    }

    // NOTE: maybe problems with unique id's
    template <class Hash, class KeyGetter>
    void deepCopySharedPointerHash(const Hash &src, Hash &dst, KeyGetter keyGetter)
    {
        dst.reserve(src.size());

        using ValueType = typename Hash::mapped_type::element_type;
        for (auto &&value : src)
            dst.insert((*value.*keyGetter)(), std::make_shared<ValueType>(*value));
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

    template <class Element>
    bool writeToFile(const Element &elem, const QString &fileName)
    {
        QFile jsonFile(fileName);

        if (jsonFile.open(QIODevice::WriteOnly)) {
            QJsonDocument jdoc(elem.toJson());
            QTextStream st(&jsonFile);
            st << jdoc.toJson();
            return true;
        }

        return false;
    }

    template <class Element>
    bool readFromFile(Element &elem, const QString &fileName)
    {
        QFile jsonFile(fileName);
        if (jsonFile.open(QIODevice::ReadOnly)) {
            QJsonParseError errorMessage;
            auto jdoc = QJsonDocument::fromJson(jsonFile.readAll(), &errorMessage);

            if (errorMessage.error == QJsonParseError::NoError) {
                ErrorList errorList;

                if (jdoc.isObject()) {
                    QJsonObject object = jdoc.object();

                    elem.fromJson(object, errorList);

                    if (errorList.isEmpty())
                        return true;
                }
            }
        }

        return false;
    }

    template <class Container, class Value>
    bool contains(const Container &c, const Value &v)
    {
        return qFind(c, v) != std::end(c);
    }

    template <class Container, class Pred>
    bool contains_if(const Container &c, Pred p)
    {
        return ranges::any_of(c, p);
    }

    template <class Container, class Val>
    void remove_erase(Container &c, const Val &v)
    {
        c.erase(std::remove(std::begin(c), std::end(c), v), std::end(c));
    }

    template <class Container, class Pred>
    void remove_erase_if(Container &c, Pred p)
    {
        c.erase(std::remove_if(std::begin(c), std::end(c), p), std::end(c));;
    }

    bool toBool(const QString &in, bool &ok);

    QString fieldKeywordToString(Entity::FieldKeyword keyword);
    Entity::FieldKeyword fieldKeywordFromString(const QString &keyword);

    QString methodLhsIdToString(Entity::LhsIdentificator id);
    Entity::LhsIdentificator methodLhsIdFromString(const QString &in);

    QString methodRhsIdToString(Entity::RhsIdentificator id);
    Entity::RhsIdentificator methodRhsIdFromString(const QString &in);

    QString sectionToString(Entity::Section section);
    QString sectionToSymbol(Entity::Section section);
}

