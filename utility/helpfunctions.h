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

#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>

#include "types.h"

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

    QStringList scopesNamesList(const entity::SharedType &type, const db::SharedDatabase &db);

    template <class D>
    std::shared_ptr<entity::Type> findType(const QString &id, const D &database)
    {
        return database ? database->depthTypeSearch(id) : nullptr;
    }

    template <class D, class... Args>
    std::shared_ptr<entity::Type> findType(const QString &id, const D &database, const Args&... args)
    {
        auto result = findType(id, database);
        return result ? result : findType(id, args...);
    }

    template <class D>
    std::shared_ptr<entity::Scope> findScope(const QString &id, const D &database)
    {
        return database ? database->depthScopeSearch(id) : nullptr;
    }

    template <class D, class... Args>
    std::shared_ptr<entity::Scope> findScope(const QString &id, const D &database, const Args&... args)
    {
        auto result = findScope(id, database);
        return result ? result : findScope(id, args...);
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

    template <class Container>
    bool seqSharedPointerEq(const Container &lhs, const Container &rhs)
    {
        if (lhs.size()  != rhs.size() ||
            typeid(lhs) != typeid(lhs))
            return false;

        using ValueType = decltype(*lhs.begin());
        return std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                          [](const ValueType &r, const ValueType &l){ return r == l || *r == *l; });
    }

    template <class Hash, class KeyGetter>
    void deepCopySharedPointerHash(const Hash &src, Hash &dst, KeyGetter keyGetter)
    {
        Hash tmpHash;
        tmpHash.reserve(src.size());

        using ValueType = typename Hash::mapped_type::element_type;
        for (auto &&value : src)
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

    template <class Type, class Interface>
    bool implements_interface()
    {
        return std::is_convertible<Type, Interface>::value;
    }

    QString fieldKeywordToString(entity::FieldKeyword keyword);
    QString methodLhsIdToString(entity::LhsIdentificator id);
    QString methodRhsIdToString(entity::RhsIdentificator id);
    QString userTypeToString(entity::UserType type);
    QString sectionToString(entity::Section section);
}

