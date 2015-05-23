/*****************************************************************************
**
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 29/10/2014.
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

#include "class.h"
#include "enums.h"
#include "field.h"
#include "classmethod.h"
#include "enums.h"
#include "extendedtype.h"
#include "constants.cpp"

#include <utility>
#include <algorithm>

#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

#include <utility/helpfunctions.h>

namespace entity {

    /**
     * @brief Class::Class
     */
    Class::Class()
        : Class(DEFAULT_NAME, GLOBAL_SCOPE_ID)
    {
    }

    /**
     * @brief Class::Class
     * @param src
     */
    Class::Class(Class &&src)
    {
        moveFrom(src);
    }

    /**
     * @brief Class::Class
     * @param src
     */
    Class::Class(const Class &src)
        : Type(src)
    {
        copyFrom(src);
    }

    /**
     * @brief Class::Class
     * @param name
     * @param scopeId
     */
    Class::Class(const QString &name, const QString &scopeId)
        : Type(name, scopeId)
        , m_Kind(Kind::ClassType)
        , m_FinalStatus(false)
    {
    }

    /**
     * @brief Class::operator =
     * @param rhs
     * @return
     */
    Class &Class::operator =(Class &&rhs)
    {
        if (this != &rhs)
            moveFrom(rhs);

        return *this;
    }

    /**
     * @brief Class::operator =
     * @param rhs
     * @return
     */
    Class &Class::operator =(Class rhs)
    {
        moveFrom(rhs);

        return *this;
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const Class &lhs, const Class &rhs)
    {
        return static_cast<const Type&>(lhs).isEqual(rhs) &&
                lhs.m_Kind        == rhs.m_Kind           &&
                lhs.m_FinalStatus == rhs.m_FinalStatus    &&
                lhs.m_Parents     == rhs.m_Parents        &&
                utility::seqSharedPointerEq(lhs.m_Methods, rhs.m_Methods) &&
                utility::seqSharedPointerEq(lhs.m_Fields,  rhs.m_Fields);
    }

    /**
     * @brief Class::addParent
     * @param typeId
     * @param section
     * @return
     */
    Parent Class::addParent(const QString &typeId, Section section)
    {
        auto parent = Parent(typeId, section);
        if (containsParent(typeId)) removeParent(typeId);
        m_Parents.append(parent);

        return parent;
    }

    /**
     * @brief Class::getParents
     * @param typeId
     * @return
     */
    ParentsList Class::getParents(const QString &typeId)
    {
        ParentsList result;

        for (auto &&p : m_Parents)
            if (p.first == typeId) result << p;

        return result;
    }

    /**
     * @brief Class::containsParent
     * @param typeId
     * @return
     */
    bool Class::containsParent(const QString &typeId)
    {
        auto it = std::find_if(m_Parents.begin(), m_Parents.end(),
                               [&](const Parent &p) { return p.first == typeId; });
        return (it != m_Parents.end());
    }

    /**
     * @brief Class::removeParent
     * @param typeId
     */
    void Class::removeParent(const QString &typeId)
    {
        auto it = std::find_if(m_Parents.begin(), m_Parents.end(),
                               [&](const Parent &p) { return p.first == typeId; });
        if (it != m_Parents.end()) m_Parents.erase(it);
    }

    /**
     * @brief Class::anyParents
     * @return
     */
    bool Class::anyParents() const
    {
        return !m_Parents.isEmpty();
    }

    /**
     * @brief Class::parents
     * @return
     */
    ParentsList Class::parents() const
    {
        return m_Parents;
    }

    /**
     * @brief Class::addMethod
     * @param method
     */
    void Class::addMethod(SharedMethod method)
    {
        method->setScopeId(scopeId());
        m_Methods << method;
    }

    /**
     * @brief Class::getMethod
     * @param name
     * @return
     */
    MethodsList Class::getMethod(const QString &name)
    {
        MethodsList result;
        for (auto &&m : m_Methods)
            if (m->name() == name)
                result << m;

        return result;
    }

    /**
     * @brief Class::containsMethod
     * @param name
     * @return
     */
    bool Class::containsMethod(const QString &name)
    {
        return !getMethod(name).isEmpty();
    }

    /**
     * @brief Class::removeMethods
     * @param name
     */
    void Class::removeMethods(const QString &name)
    {
        auto methods = getMethod(name);
        for (auto &&m : methods) m_Methods.removeAll(m);
    }

    /**
     * @brief Class::removeMethod
     * @param method
     */
    void Class::removeMethod(const SharedMethod &method)
    {
        m_Methods.removeOne(method);
    }

    /**
     * @brief Class::methods
     * @return
     */
    MethodsList Class::methods() const
    {
        return m_Methods;
    }

    /**
     * @brief Class::anyMethods
     * @return
     */
    bool Class::anyMethods() const
    {
        return !m_Methods.isEmpty();
    }

    /**
     * @brief Class::containsMethods
     * @param section
     * @return
     */
    bool Class::containsMethods(Section section) const
    {
        bool result(false);

        for (auto &&method : m_Methods) {
            if (method->section() == section) {
                result = true;
                break;
            }
        }

        return result;
    }

    /**
     * @brief Class::methods
     * @param section
     * @return
     */
    MethodsList Class::methods(Section section) const
    {
        MethodsList result;

        for (auto &&method : m_Methods)
            if (method->section() == section) result << method;

        return result;
    }

    /**
     * @brief Class::addField
     * @param name
     * @param typeId
     * @param prefix
     * @param section
     * @return
     */
    SharedField Class::addField(const QString &name, const QString &typeId, const QString &prefix, Section section)
    {
        auto field = std::make_shared<Field>(name, typeId, prefix, section);
        if (containsField(name))
            removeField(name);
        m_Fields.append(field);

        return field;
    }

    /**
     * @brief Class::getField
     * @param name
     * @return
     */
    SharedField Class::getField(const QString &name) const
    {
        auto it = std::find_if(m_Fields.begin(), m_Fields.end(),
                               [&name](const SharedField &f){ return f->name() == name; });
        return (it != m_Fields.end() ? *it : nullptr);
    }

    /**
     * @brief Class::containsField
     * @param name
     * @return
     */
    bool Class::containsField(const QString &name) const
    {
        return getField(name).operator bool();
    }

    /**
     * @brief Class::removeField
     * @param name
     */
    void Class::removeField(const QString &name)
    {
        auto f = getField(name);
        if (f) m_Fields.removeAt(m_Fields.indexOf(f));
    }

    /**
     * @brief Class::fields
     * @return
     */
    FieldsList Class::fields() const
    {
        return m_Fields;
    }

    /**
     * @brief Class::anyFields
     * @return
     */
    bool Class::anyFields() const
    {
        return !m_Fields.empty();
    }

    /**
     * @brief Class::containsFields
     * @param section
     * @return
     */
    bool Class::containsFields(Section section) const
    {
        bool result(false);

        for (auto &&field : m_Fields) {
            if (field->section() == section) {
                result = true;
                break;
            }
        }

        return result;
    }

    /**
     * @brief Class::fields
     * @param section
     * @return
     */
    FieldsList Class::fields(Section section) const
    {
        FieldsList result;

        for (auto &&field : m_Fields)
            if (field->section() == section) result << field;

        return result;
    }

    /**
     * @brief Class::kind
     * @return
     */
    Kind Class::kind() const
    {
        return m_Kind;
    }

    /**
     * @brief Class::setKind
     * @param kind
     */
    void Class::setKind(Kind kind)
    {
        m_Kind = kind;
    }

    /**
     * @brief Class::isFinal
     * @return
     */
    bool Class::isFinal() const
    {
        return m_FinalStatus;
    }

    /**
     * @brief Class::setFinalStatus
     * @param status
     */
    void Class::setFinalStatus(bool status)
    {
        m_FinalStatus = status;
    }

    /**
     * @brief Class::toJson
     * @return
     */
    QJsonObject Class::toJson() const
    {
        QJsonObject result(Type::toJson());

        result.insert("Kind", m_Kind);
        result.insert("Final status", m_FinalStatus);

        QJsonArray parents;
        QJsonObject parent;
        for (auto &&p: m_Parents) {
            parent.insert("Id", p.first);
            parent.insert("Section", p.second);
            parents.append(parent);
        }
        result.insert("Parents", parents);

        QJsonArray methods;
        for (auto &&value : m_Methods) methods.append(value->toJson());
        result.insert("Methods", methods);

        QJsonArray fields;
        for (auto &&value : m_Fields) fields.append(value->toJson());
        result.insert("Fields", fields);

        return result;
    }

    /**
     * @brief Class::fromJson
     * @param src
     * @param errorList
     */
    void Class::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        Type::fromJson(src, errorList);

        utility::checkAndSet(src, "Kind", errorList, [&src, this](){
            m_Kind = static_cast<Kind>(src["Kind"].toInt());
        });
        utility::checkAndSet(src, "Final status", errorList, [&src, this](){
            m_FinalStatus = src["Final status"].toBool();
        });

        m_Parents.clear();
        utility::checkAndSet(src, "Parents", errorList, [&src, &errorList, this](){
            Parent p;
            QJsonObject o;
            if (src["Parents"].isArray()) {
                for (auto &&value : src["Parents"].toArray()) {
                    o = value.toObject();
                    utility::checkAndSet(o, "Id", errorList, [&o, &p, this](){
                        p.first = o["Id"].toString();
                    });
                    utility::checkAndSet(o, "Section", errorList, [&o, &p, this](){
                        p.second = static_cast<Section>(o["Section"].toInt());
                    });
                    m_Parents.append(p);
                }
            } else {
                errorList << "Error: \"Parents\" is not array";
            }
        });

        m_Methods.clear();
        utility::checkAndSet(src, "Methods", errorList, [&src, &errorList, this](){
            if (src["Methods"].isArray()) {
                SharedMethod method;
                QJsonObject obj;
                for (auto &&value : src["Methods"].toArray()) {
                    obj = value.toObject();
                    utility::checkAndSet(obj, "Type", errorList,
                                         [&obj, &errorList, &method, this](){
                        method = utility::makeMethod(static_cast<ClassMethodType>(obj["Type"].toInt()));
                        method->fromJson(obj, errorList);
                        m_Methods << method;
                    });
                }
            } else {
                errorList << "Error: \"Methods\" is not array";
            }
        });

        m_Fields.clear();
        utility::checkAndSet(src, "Fields", errorList, [&src, &errorList, this](){
            if (src["Fields"].isArray()) {
                SharedField field;
                for (auto &&value : src["Fields"].toArray()) {
                    field = std::make_shared<Field>();
                    field->fromJson(value.toObject(), errorList);
                    m_Fields << field;
                }
            } else {
                errorList << "Error: \"Fields\" is not array";
            }
        });
    }

    /**
     * @brief Class::hashType
     * @return
     */
    size_t Class::hashType() const
    {
        return Class::staticHashType();
    }

    /**
     * @brief Class::staticHashType
     * @return
     */
    size_t Class::staticHashType()
    {
        return typeid(Class).hash_code();
    }

    /**
     * @brief Class::isEqual
     * @param rhs
     * @return
     */
    bool Class::isEqual(const Class &rhs) const
    {
        return *this == rhs;
    }

    /**
     * @brief Class::textMarker
     * @return
     */
    QString Class::textMarker() const
    {
        return "class";
    }

    /**
     * @brief Class::moveFrom
     * @param src
     */
    void Class::moveFrom(Class &src)
    {
        Type::moveFrom(src);

        m_Kind = std::move(src.m_Kind);
        m_FinalStatus = std::move(src.m_FinalStatus);

        m_Parents = std::move(src.m_Parents);
        m_Methods = std::move(src.m_Methods);
        m_Fields  = std::move(src.m_Fields );
    }

    /**
     * @brief Class::copyFrom
     * @param src
     */
    void Class::copyFrom(const Class &src)
    {
        m_Kind = src.m_Kind;
        m_FinalStatus = src.m_FinalStatus;
        m_Parents = src.m_Parents;

        utility::deepCopySharedPointerList(src.m_Methods, m_Methods);
        utility::deepCopySharedPointerList(src.m_Fields,  m_Fields );
    }

} // namespace entity
