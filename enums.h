#pragma once

namespace entity {
    enum Section  : int {Public, Private, Protected};
    enum Kind     : int {ClassType, StructType};
    enum UserType : int {BasicType, UserClassType, TemplateClassType, UnionType, EnumType};
    enum RhsIdentificator : int {None, Override, Final, Delete, PureVirtual};
    enum LhsIdentificator : int {Explicit, Inline, MethodStatic, Virtual, Friend};
    enum FieldKeyword     : int {Volatile, Mutable, FieldStatic};
}
