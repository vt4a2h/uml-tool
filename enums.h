#pragma once

namespace entity {
    enum Section  : int {Public, Private, Protected};
    enum Kind     : int {ClassType, StructType};
    enum UserType : int {BasicType, UserClassType, TemplateClassType,
                         UnionType, EnumType, ExtendedTypeType};
    enum RhsIdentificator : int {None, Override, Final, Delete, PureVirtual};
    enum LhsIdentificator : int {Explicit, Inline, MethodStatic, Virtual, Friend};
    enum FieldKeyword     : int {Volatile, Mutable, FieldStatic};
    enum ClassMethodType  : int {SimpleMethod, TemplateMethod};
}

namespace relationship {
    enum Multiplicity : int {NotSpecified, ZeroOrOne, One, ZeroOrInf, OneOrInf, Inf};
    enum RelationType : int {SimpleRelation, AssociationRelation, DependencyRelation,
                             GeneralizationRelation, RealizationRelation, CompositionRelation,
                             AggregationRelation, MultiRelation};
}
