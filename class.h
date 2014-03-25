#pragma once

#include "type.h"

namespace entity {

    enum Kind : int;

    class Class : public Type
    {
    public:
        Class();

        Kind kind() const;
        void setKind(Kind kind);

    private:
        Kind m_Kind;
    };

} // namespace entity
