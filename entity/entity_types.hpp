/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 10/05/2015.
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

#include <QtContainerFwd>
#include <QMetaType>

namespace entity {
    class EntityID;

    enum FieldKeyword : int;
    using FieldKeywords     = QSet<FieldKeyword>;
    using FieldKeywordsList = QVector<FieldKeyword>;

    class Property;
    using SharedProperty = std::shared_ptr<Property>;
    using ConstSharedProperty = std::shared_ptr<const Property>;
    using PropertiesList = QVector<SharedProperty>;

    class ClassMethod;
    using SharedMethod = std::shared_ptr<ClassMethod>;
    using WeakMethod   = std::weak_ptr<ClassMethod>;
    using Methods      = QMultiHash<QString, SharedMethod>;
    using MethodsList  = QVector<SharedMethod>;
    using WeakMethodsList = QVector<WeakMethod>;
    using WeakMethodsMap = QMap<SharedProperty, WeakMethodsList>;

    class Field;
    using SharedField = std::shared_ptr<Field>;
    using Fields      = QHash<QString, SharedField>;
    using FieldsList  = QVector<SharedField>;
    using WeakField   = std::weak_ptr<Field>;
    using WeakFieldsList = QVector<WeakField>;
    using WeakFieldsMap = QMap<SharedProperty, WeakFieldsList>;

    class TemplateClassMethod;
    using SharedTemplateClassMethod = std::shared_ptr<TemplateClassMethod>;

    enum class LhsIdentificator : int;
    using LhsIdentificators     = QSet<LhsIdentificator>;
    using LhsIdentificatorsList = QVector<LhsIdentificator>;

    class Type;
    using SharedType  = std::shared_ptr<Type>;
    using Types       = QHash<EntityID, SharedType>;
    using TypesByName = QHash<QString, SharedType>;
    using TypesList   = QVector<SharedType>;

    class Class;
    using SharedClass = std::shared_ptr<Class>;
    using Classes     = QHash<QString, SharedClass>;
    using ClassesList = QVector<SharedClass>;

    class TemplateClass;
    using SharedTemplateClass = std::shared_ptr<TemplateClass>;

    class Template;
    using SharedTemplate = std::shared_ptr<Template>;

    class Union;
    using SharedUnion = std::shared_ptr<Union>;
    using Unions      = QHash<QString, SharedUnion>;
    using UnionsList  = QVector<SharedUnion>;

    class Enum;
    using SharedEnum = std::shared_ptr<Enum>;
    using Enums      = QHash<QString, SharedEnum>;
    using EnumsList  = QVector<SharedEnum>;

    enum Section : int;
    using Parent      = std::pair<EntityID, Section>;
    using Parents     = QHash<QString, Parent>; // id : {id, section}
    using ParentsList = QVector<Parent>;

    class ExtendedType;
    using TemplateParameter      = QPair<EntityID, EntityID>; // (type id, default type id)
    using TemplateParameters     = QHash<QString, TemplateParameter>;
    using TemplateParametersList = QVector<TemplateParameter>;
    using SharedExtendedType     = std::shared_ptr<ExtendedType>;
    using ExtendedTypes          = QHash<QString, SharedExtendedType>;
    using ExtendedTypesList      = QVector<SharedExtendedType>;
    using ExtendedTypesIdList    = QVector<EntityID>;

    class Scope;
    using SharedScope = std::shared_ptr<Scope>;
    using Scopes      = QHash<EntityID, SharedScope>;
    using ScopesList  = QVector<SharedScope>;

    class IComponents;
    using SharedComponents = std::shared_ptr<IComponents>;

    class Enumerator;
    using SharedEnumarator = std::shared_ptr<Enumerator>;
    using Enumerators = QVector<SharedEnumarator>;

    struct Member;
    using SharedMember = std::shared_ptr<Member>;

    class ITypeUser;
    using SharedTypeUser = std::shared_ptr<ITypeUser>;
}

Q_DECLARE_METATYPE(entity::SharedField)
Q_DECLARE_METATYPE(entity::SharedMethod)
Q_DECLARE_METATYPE(entity::SharedType)
Q_DECLARE_METATYPE(entity::SharedClass)
Q_DECLARE_METATYPE(entity::SharedScope)
Q_DECLARE_METATYPE(entity::SharedProperty)
