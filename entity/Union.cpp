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
#include "Union.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

#include <boost/range/algorithm/find_if.hpp>

#include <utility/helpfunctions.h>

#include "field.h"
#include "ExtendedType.h"
#include "constants.h"
#include "enums.h"

using namespace boost;

namespace entity {

    namespace {
        const QString newFiledName = Union::tr("newField");
        const QString defaultName = Union::tr("Union");
    }

    /**
     * @brief Union::Union
     */
    Union::Union()
        : Union(defaultName, common::ID::globalScopeID())
    {
    }

    /**
     * @brief Union::Union
     * @param src
     */
    Union::Union(const Union &src)
        : Type(src)
    {
        copyFrom(src);
    }

    /**
     * @brief Union::Union
     * @param name
     * @param scopeId
     */
    Union::Union(const QString &name, const common::ID &scopeId)
        : Type(name.isEmpty() ? defaultName : name, scopeId)
    {
        m_KindOfType = KindOfType::Union;
    }

    /**
     * @brief Union::operator =
     * @param rhs
     * @return
     */
    Union &Union::operator=(const Union &rhs)
    {
        if (this != &rhs) {
            Type::operator =(rhs);
            copyFrom(rhs);
        }

        return *this;
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const Union &lhs, const Union &rhs)
    {
        return lhs.isEqual(rhs);
    }

    /**
     * @brief Union::getField
     * @param name
     * @return
     */
    SharedField Union::getField(const QString &name) const
    {
        auto it = range::find_if(m_Fields, [&](auto &&f) {return f->name() == name;});
        return it != m_Fields.cend() ? *it : SharedField();
    }

    /**
     * @brief Union::addField
     * @param name
     * @param typeId
     * @return
     */
    SharedField Union::addField(const QString &name, const common::ID &typeId)
    {
        auto field = std::make_shared<Field>(name, typeId);

        if (getField(name) != nullptr) removeField(name);
        m_Fields.append(field);

        return field;
    }

    /**
     * @brief Union::removeField
     * @param name
     */
    void Union::removeField(const QString &name)
    {
        auto field = getField(name);
        if (field)
            m_Fields.remove(m_Fields.indexOf(field));
    }

    /**
     * @brief Union::containsField
     * @param name
     * @return
     */
    bool Union::containsField(const QString &name)
    {
        return (getField(name) != nullptr);
    }

    /**
     * @brief Union::fields
     * @return
     */
    FieldsList Union::fields() const
    {
        return m_Fields;
    }

    /**
     * @brief Union::toJson
     * @return
     */
    QJsonObject Union::toJson() const
    {
        QJsonObject result(Type::toJson());

        QJsonArray fields;
        for (auto &&value : m_Fields) fields.append(value->toJson());
        result.insert("Fields", fields);

        return result;
    }

    /**
     * @brief Union::fromJson
     * @param src
     * @param errorList
     */
    void Union::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        Type::fromJson(src, errorList);

        m_Fields.clear();
        utility::checkAndSet(src, "Fields", errorList, [&src, &errorList, this](){
            if (src["Fields"].isArray()) {
                SharedField f;
                for (auto &&value : src["Fields"].toArray()) {
                    f = std::make_shared<Field>();
                    f->fromJson(value.toObject(), errorList);
                    m_Fields.append(f);
                }
            } else {
                errorList << "Error: \"Fields\" is not array";
            }
        });
    }

    /**
     * @brief Union::isEqual
     * @param rhs
     * @return
     */
    bool Union::isEqual(const Type &rhs, bool withTypeid) const
    {
        if (!Type::isEqual(rhs, withTypeid))
            return false;

        auto r = static_cast<const Union &>(rhs);
        return utility::seqSharedPointerEq(m_Fields, r.m_Fields);
    }

    /**
     * @brief Union::addNewFiled
     * @return
     */
    SharedField Union::addNewField()
    {
        return addField(newFiledName, common::ID::nullID());
    }

    /**
     * @brief Union::addExistsFiled
     * @param field
     * @param pos
     */
    void Union::addExistsField(const SharedField &field, int pos)
    {
        if (pos == -1)
            m_Fields << field;
        else
            m_Fields.insert(pos, field);
    }

    /**
     * @brief Union::removeField
     * @param field
     * @return
     */
    int Union::removeField(const SharedField &field)
    {
        int pos = m_Fields.indexOf(field);
        m_Fields.remove(pos);
        return pos;
    }

    /**
     * @brief Union::copyFrom
     * @param src
     */
    void Union::copyFrom(const Union &src)
    {
        utility::deepCopySharedPointerList(src.m_Fields, m_Fields);
    }

} // namespace entity
