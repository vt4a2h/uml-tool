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

namespace entity {
    /**
     * @brief The Section enum
     */
    enum Section  : int {Public, Private, Protected};
    /**
     * @brief The Kind enum
     */
    enum Kind     : int {ClassType, StructType};
    /**
     * @brief The UserType enum
     */
    enum UserType : int {BasicType, UserClassType, TemplateClassType,
                         UnionType, EnumType, ExtendedTypeType};
    /**
     * @brief The RhsIdentificator enum
     */
    enum RhsIdentificator : int {None, Override, Final, Delete, Default, PureVirtual};
    /**
     * @brief The LhsIdentificator enum
     */
    enum LhsIdentificator : int {Explicit, Inline, MethodStatic, Virtual, Friend};
    /**
     * @brief The FieldKeyword enum
     */
    enum FieldKeyword     : int {Volatile, Mutable, FieldStatic};
    /**
     * @brief The ClassMethodType enum
     */
    enum ClassMethodType  : int {SimpleMethod, TemplateMethod};
}

namespace relationship {
    /**
     * @brief The Multiplicity enum
     */
    enum Multiplicity : int {NotSpecified, ZeroOrOne, One, ZeroOrInf, OneOrInf, Inf};
    /**
     * @brief The RelationType enum
     */
    enum RelationType : int {SimpleRelation, AssociationRelation, DependencyRelation,
                             GeneralizationRelation, RealizationRelation, CompositionRelation,
                             AggregationRelation, MultiRelation};
}
