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
#include <utility>
#include <algorithm>

#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

#include <utility/helpfunctions.h>

#include "class.h"
#include "enums.h"
#include "field.h"
#include "classmethod.h"
#include "enums.h"
#include "extendedtype.h"
#include "property.h"
#include "constants.h"

#include "qthelpers.h"

using namespace boost;

namespace {
    const QString newMethodName = entity::Class::tr("newMethod");
    const QString newFieldName  = entity::Class::tr("newField");
    const QString newPropertyName = entity::Class::tr("newProperty");
}

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
    Class &Class::operator =(const Class &rhs)
    {
        if (this != &rhs)
            copyFrom(rhs);

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
        return lhs.isEqual(rhs);
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
        if (containsParent(typeId))
            removeParent(typeId);
        m_Parents.append(parent);

        return parent;
    }

    /**
     * @brief Class::parent
     * @param typeId
     * @return
     */
    Parent Class::parent(const QString &typeId) const
    {
        auto it = range::find_if(m_Parents, [&](auto &&parent){ return parent.first == typeId; });
        return it != m_Parents.cend() ? *it : Parent();
    }

    /**
     * @brief Class::containsParent
     * @param typeId
     * @return
     */
    bool Class::containsParent(const QString &typeId)
    {
        return range::find_if(m_Parents, [&](const Parent &p) { return p.first == typeId; }) != m_Parents.end();
    }

    /**
     * @brief Class::removeParent
     * @param typeId
     */
    void Class::removeParent(const QString &typeId)
    {
        auto it = range::find_if(m_Parents, [&](const Parent &p) { return p.first == typeId; });
        if (it != m_Parents.end())
            m_Parents.erase(it);
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
    int Class::removeMethod(const SharedMethod &method)
    {
        int pos = m_Methods.indexOf(method);
        m_Methods.removeOne(method);
        return pos;
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
     * @brief Class::addNewFiled
     * @return
     */
    SharedField Class::addNewField()
    {
        return addField(newFieldName, STUB_ID);
    }

    /**
     * @brief Class::addExistsFiled
     * @param field
     * @param pos
     */
    void Class::addExistsField(const SharedField &field, int pos)
    {
        if (pos == -1)
            m_Fields << field;
        else
            m_Fields.insert(pos, field);
    }

    /**
     * @brief Class::removeField
     * @param field
     * @return
     */
    int Class::removeField(const SharedField &field)
    {
        int pos = m_Fields.indexOf(field);
        m_Fields.removeOne(field);
        return pos;
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
        return cend(m_Methods) != range::find_if(m_Methods, [&](auto &&method){ return method->section() == section; });
    }

    /**
     * @brief Class::methods
     * @param section
     * @return
     */
    MethodsList Class::methods(Section s) const
    {
        MethodsList result;
        range::copy(m_Methods | adaptors::filtered([&](auto &&m){ return m->section() == s || s == All; }),
                    std::back_inserter(result));

        return result;
    }

    /**
     * @brief Class::optionalMethods
     * @param s
     * @return
     */
    MethodsList Class::optionalMethods(Section s) const
    {
        using namespace adaptors;

        MethodsList result;

        for (auto &&methods : m_OptionalMethods.values())
            range::copy(methods
                        | filtered([&](auto &&m){ return m.lock() && (m.lock()->section() == s || s == All); })
                        | transformed([](auto &&m){ return m.lock(); }),
                        std::back_inserter(result));

        return result;
    }

    /**
     * @brief Class::allMethods
     * @param s
     * @return
     */
    MethodsList Class::allMethods(Section s) const
    {
        MethodsList result;
        result << methods(s) << optionalMethods(s);

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
        auto it = range::find_if(m_Fields, [&name](const SharedField &f){ return f->name() == name; });
        return it != m_Fields.end() ? *it : SharedField();
    }

    /**
     * @brief Class::containsField
     * @param name
     * @return
     */
    bool Class::containsField(const QString &name) const
    {
        return getField(name) != nullptr;
    }

    /**
     * @brief Class::removeField
     * @param name
     */
    void Class::removeField(const QString &name)
    {
        if (auto f = getField(name))
            m_Fields.removeAt(m_Fields.indexOf(f));
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
     * @brief Class::addNewProperty
     * @return
     */
    SharedProperty Class::addNewProperty()
    {
        return addProperty(newPropertyName, STUB_ID);
    }

    /**
     * @brief Class::addExistsProperty
     * @param property
     * @param pos
     */
    void Class::addExistsProperty(const SharedProperty &property, int pos)
    {
        if (pos == -1)
            m_Properties << property;
        else
            m_Properties.insert(pos, property);
    }

    /**
     * @brief Class::removeProperty
     * @param property
     * @return
     */
    int Class::removeProperty(const SharedProperty &property)
    {
        int pos = m_Properties.indexOf(property);
        if (pos != -1) {
            disconnect(property.get(), &Property::methodAdded, this, &Class::onOptionalMethodAdded);
            disconnect(property.get(), &Property::methodRemoved, this, &Class::onOptionalMethodRemoved);
            m_Properties.removeOne(property);
        }

        return pos;
    }

    /**
     * @brief Class::properties
     * @return
     */
    PropertiesList Class::properties() const
    {
        return m_Properties;
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
     * @brief Class::addProperty
     * @param name
     * @param typeId
     * @return
     */
    SharedProperty Class::addProperty(const QString &name, const QString &typeId)
    {
        SharedProperty property(std::make_shared<Property>(name, typeId));
        G_CONNECT(property.get(), &Property::methodAdded, this, &Class::onOptionalMethodAdded);
        G_CONNECT(property.get(), &Property::methodRemoved, this, &Class::onOptionalMethodRemoved);
        G_CONNECT(property.get(), &Property::fieldAdded, this, &Class::onOptionalFieldAdded);
        G_CONNECT(property.get(), &Property::fieldRemoved, this, &Class::onOptionalFieldRemoved);

        m_OptionalFields[property] << property->field();

        m_Properties.append(property);

        return property;
    }

    /**
     * @brief Class::property
     * @param name
     * @return
     */
    ConstSharedProperty Class::property(const QString &name) const
    {
        auto it = range::find_if(m_Properties, [&](auto &&prop){ return prop->name() == name; });
        return it != m_Properties.end() ? *it : SharedProperty();
    }

    /**
     * @brief Class::property
     * @param name
     * @return
     */
    SharedProperty Class::property(const QString &name)
    {
        return std::const_pointer_cast<Property>(const_cast<const Class *>(this)->property(name));
    }

    /**
     * @brief Class::containsProperty
     * @param name
     * @return
     */
    bool Class::containsProperty(const QString &name) const
    {
        return property(name) != nullptr;
    }

    /**
     * @brief Class::removeProperty
     * @param name
     */
    void Class::removeProperty(const QString &name)
    {
        if (auto prop = property(name)) {
            G_DISCONNECT(prop.get(), &Property::methodAdded, this, &Class::onOptionalMethodAdded);
            G_DISCONNECT(prop.get(), &Property::methodRemoved, this, &Class::onOptionalMethodRemoved);
            G_DISCONNECT(prop.get(), &Property::fieldAdded, this, &Class::onOptionalFieldAdded);
            G_DISCONNECT(prop.get(), &Property::fieldRemoved, this, &Class::onOptionalFieldRemoved);

            m_Properties.removeOne(prop);
            m_OptionalMethods.remove(prop);
            m_OptionalFields.remove(prop);
        }
    }

    /**
     * @brief Class::anyProperties
     * @return
     */
    bool Class::anyProperties() const
    {
        return !m_Properties.empty();
    }

    /**
     * @brief Class::containsFields
     * @param section
     * @return
     */
    bool Class::containsFields(Section section) const
    {
        return range::find_if(m_Fields, [&](auto &&f){ return f->section() == section; }) !=
               cend(m_Fields);
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
            if (field->section() == section)
                result << field;

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
        for (auto &&value : m_Methods)
            methods.append(value->toJson());
        result.insert("Methods", methods);

        QJsonArray fields;
        for (auto &&value : m_Fields)
            fields.append(value->toJson());
        result.insert("Fields", fields);

        QJsonArray properties;
        for (auto &&value : m_Properties)
            properties.append(value->toJson());
        result.insert("Properties", properties);

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

        m_Properties.clear();
        utility::checkAndSet(src, "Properties", errorList, [&src, &errorList, this](){
            if (src["Properties"].isArray()) {
                SharedProperty property;
                for (auto &&value : src["Properties"].toArray()) {
                    property = std::make_shared<Property>();
                    property->fromJson(value.toObject(), errorList);
                    m_Properties << property;
                }
            } else {
                errorList << "Error: \"Properties\" is not array";
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
     * @brief Class::marker
     * @return
     */
    QString Class::marker() const
    {
        return Class::staticMarker();
    }

    /**
     * @brief Class::staticMarker
     * @return
     */
    QString Class::staticMarker()
    {
        return "Class";
    }

    /**
     * @brief Class::isEqual
     * @param rhs
     * @return
     */
    bool Class::isEqual(const Type &rhs, bool withTypeid) const
    {
        if (!Type::isEqual(rhs, withTypeid))
            return false;

        auto r = static_cast<const Class &>(rhs);
        return m_Kind        == r.m_Kind           &&
               m_FinalStatus == r.m_FinalStatus    &&
               m_Parents     == r.m_Parents        &&
               utility::seqSharedPointerEq(m_Methods, r.m_Methods) &&
               utility::seqSharedPointerEq(m_Fields,  r.m_Fields)  &&
               utility::seqSharedPointerEq(m_Properties, r.m_Properties) &&
               utility::seqSharedPointerEq(optionalMethods(None), r.optionalMethods(None));
    }

    /**
     * @brief Class::addNewMethod
     * @return
     */
    SharedMethod Class::addNewMethod()
    {
        return makeMethod(newMethodName);
    }

    /**
     * @brief Class::addExistsMethod
     * @param method
     */
    void Class::addExistsMethod(const SharedMethod &method, int pos)
    {
        method->setScopeId(scopeId());
        if (pos == -1)
            m_Methods << method;
        else
            m_Methods.insert(pos, method);
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
        m_Properties = std::move(src.m_Properties);

        m_OptionalMethods = std::move(src.m_OptionalMethods);
        m_OptionalFields = std::move(src.m_OptionalFields);
    }

    /**
     * @brief Class::copyFrom
     * @param src
     */
    void Class::copyFrom(const Class &src)
    {
        Type::copyFrom(src);

        m_Kind = src.m_Kind;
        m_FinalStatus = src.m_FinalStatus;
        m_Parents = src.m_Parents;

        utility::deepCopySharedPointerList(src.m_Methods, m_Methods);
        utility::deepCopySharedPointerList(src.m_Fields,  m_Fields );
        utility::deepCopySharedPointerList(src.m_Properties, m_Properties);
        m_OptionalMethods = src.m_OptionalMethods;
        m_OptionalFields = src.m_OptionalFields;
    }

    namespace
    {
        template <class Container, class Entity>
        inline void addOptionalEntity(Container &c, const entity::SharedProperty &p, const Entity &e)
        {
            if (e) {
                Q_ASSERT(c[G_ASSERT(p)].indexOf(e) == -1);
                c[p] << e;
            }
        }

        template <class Container, class Entity>
        inline void removeOptionalEntity(Container &c, const entity::SharedProperty &p, const Entity &e)
        {
            if (G_ASSERT(e)) {
                Q_ASSERT(c[G_ASSERT(p)].indexOf(e) != -1);
                c[p].removeOne(e);

                // Also remove possible NULL pointers
                range::remove_erase_if(c[p], [](auto p){ return !p.lock(); });
            }
        }
    }

    /**
     * @brief Class::onOptionalMethodAdded
     * @param p
     * @param m
     */
    void Class::onOptionalMethodAdded(const entity::SharedProperty &p, const SharedMethod &m)
    {
        addOptionalEntity(m_OptionalMethods, p, m);
    }

    /**
     * @brief Class::onOptionalMethodRemoved
     * @param p
     * @param m
     */
    void Class::onOptionalMethodRemoved(const SharedProperty &p, const SharedMethod &m)
    {
        removeOptionalEntity(m_OptionalMethods, p, m);
    }

    /**
     * @brief Class::onOptionalFieldAdded
     * @param p
     * @param f
     */
    void Class::onOptionalFieldAdded(const SharedProperty &p, const SharedField &f)
    {
        addOptionalEntity(m_OptionalFields, p, f);
    }

    /**
     * @brief Class::onOptionalFieldRemoved
     * @param p
     * @param f
     */
    void Class::onOptionalFieldRemoved(const SharedProperty &p, const SharedField &f)
    {
        removeOptionalEntity(m_OptionalFields, p, f);
    }

} // namespace entity
