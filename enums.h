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
