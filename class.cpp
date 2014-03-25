#include "class.h"
#include "enums.h"

namespace entity {

    Class::Class()
    {
    }

    Kind Class::kind() const
    {
        return m_Kind;
    }

    void Class::setKind(Kind kind)
    {
        m_Kind = kind;
    }


} // namespace entity
