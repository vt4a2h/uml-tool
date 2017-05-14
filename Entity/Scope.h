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
#pragma once

#include <type_traits>

#include <QString>
#include <QHash>

#include <Common/BasicElement.h>

#include <Helpers/entityhelpres.h>

#include "entity_types.hpp"
#include "QtHelpers.h"
#include "itypeuser.h"

namespace Entity {

    enum UserType : int;

    /**
     * @brief The Scope class
     */
    class Scope : public Common::BasicElement
    {
        Q_OBJECT

    public:
        Scope(Scope &&src);
        Scope(const Scope &src);
        Scope(const QString &scopeName = "", const Common::ID &parentScopeID = Common::ID::nullID());

        Scope &operator =(const Scope &rhs);
        Scope &operator =(Scope &&rhs);

        friend bool operator ==(const Scope &lhs, const Scope &rhs);

        SharedType type(const Common::ID &typeId) const;
        SharedType type(const Common::ID &typeId);

        SharedType type(const QString &name) const;
        SharedType type(const QString &name) ;

        template <class T = Type> std::shared_ptr<T> addType(const QString &name = "");
        SharedType addExistsType(const SharedType &type);
        bool containsType(const Common::ID &typeId) const;
        bool containsType(const QString &name) const;
        void removeType(const Common::ID &typeId);
        TypesList types() const;

        SharedScope getChildScope(const Common::ID &typeId);
        SharedScope addChildScope(const QString &name = "");
        SharedScope chainScopeSearch(const QStringList& scopesNames) const;
        void addExistsChildScope(const SharedScope &scope);
        bool containsChildScope(const Common::ID &typeId) const;
        bool hasChildScopes() const;
        void removeChildScope(const Common::ID &typeId);
        ScopesList scopes() const;

    public: // BasicEntity implementation
        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

    public slots:
        void onTypeNameChanged(const QString &oldName, const QString &newName);
        void onTypeIdChanged(const Common::ID &oldID, const Common::ID &newID);
        void onTemplateMethodAdded(const SharedTemplateClassMethod &m);

    signals:
        void typeSearcherRequired(const SharedTypeUser &);

    private:
        void copyFrom(const Scope &src);
        void moveFrom(Scope &&src);

        void connectType(Type * t);
        void connectTemplate(Template *t);

        Scopes m_Scopes;
        Types  m_Types;
        TypesByName  m_TypesByName;
    };

    template <class T>
    std::shared_ptr<T> Scope::addType(const QString &name)
    {
        using ResultType = typename std::conditional<std::is_class<T>::value &&
                                                     std::is_base_of<Type, T>::value,
                                                     T, Type>::type;
        auto value = std::make_shared<ResultType>(name, m_Id);
        uniquifyName(*value, m_TypesByName.keys());

        m_Types[value->id()] = value;
        m_TypesByName[value->name()] = value;

        connectType(value.get());

        return value;
    }

} // namespace entity

Q_DECLARE_METATYPE(Entity::Scope)
