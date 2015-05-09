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

#include <QHash>
#include <QStringList>

namespace entity {
    class BasicEntity;
    using SharedBasicEntity = std::shared_ptr<BasicEntity>;

    class Field;
    using SharedField = std::shared_ptr<Field>;
    using Fields      = QHash<QString, SharedField>;
    using FieldsList  = QList<SharedField>;

    enum FieldKeyword : int;
    using FieldKeywords     = QSet<FieldKeyword>;
    using FieldKeywordsList = QList<FieldKeyword>;

    class ClassMethod;
    using SharedMethod = std::shared_ptr<ClassMethod>;
    using Methods      = QMultiHash<QString, SharedMethod>;
    using MethodsList  = QList<SharedMethod>;

    class TemplateClassMethod;
    using SharedTemplateClassMethod = std::shared_ptr<TemplateClassMethod>;

    enum  LhsIdentificator : int;
    using LhsIdentificators     = QSet<LhsIdentificator>;
    using LhsIdentificatorsList = QList<LhsIdentificator>;

    class Type;
    using SharedType = std::shared_ptr<Type>;
    using Types      = QHash<QString, SharedType>;
    using TypesList  = QList<SharedType>;

    class Class;
    using SharedClass = std::shared_ptr<Class>;
    using Classes     = QHash<QString, SharedClass>;
    using ClassesList = QList<SharedClass>;

    class TemplateClass;
    using SharedTemplateClass = std::shared_ptr<TemplateClass>;

    class Template;
    using SharedTemplate = std::shared_ptr<Template>;

    class Union;
    using SharedUnion = std::shared_ptr<Union>;
    using Unions      = QHash<QString, SharedUnion>;
    using UnionsList  = QList<SharedUnion>;

    class Enum;
    using SharedEnum = std::shared_ptr<Enum>;
    using Enums      = QHash<QString, SharedEnum>;
    using EnumsList  = QList<SharedEnum>;

    enum Section : int;
    using Parent      = std::pair<QString, Section>;
    using Parents     = QHash<QString, Parent>; // id : {id, section}
    using ParentsList = QList<Parent>;

    class ExtendedType;
    using TemplateParameter      = QPair<QString, QString>; // (type id, default type id)
    using TemplateParameters     = QHash<QString, TemplateParameter>;
    using TemplateParametersList = QList<TemplateParameter>;
    using SharedExtendedType     = std::shared_ptr<ExtendedType>;
    using ExtendedTypes          = QHash<QString, SharedExtendedType>;
    using ExtendedTypesList      = QList<SharedExtendedType>;
    using ExtendedTypesIdList    = QStringList;

    class Scope;
    using SharedScope = std::shared_ptr<Scope>;
    using Scopes      = QHash<QString, SharedScope>;
    using ScopesList  = QList<SharedScope>;
}

Q_DECLARE_METATYPE(entity::SharedField)
Q_DECLARE_METATYPE(entity::SharedMethod)
Q_DECLARE_METATYPE(entity::SharedType)
Q_DECLARE_METATYPE(entity::SharedClass)
Q_DECLARE_METATYPE(entity::SharedScope)
