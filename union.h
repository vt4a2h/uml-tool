#pragma once

#include "type.h"
#include "types.h"

namespace entity {

    class Field;
    class ExtendedType;

    class Union : public Type
    {
    public:
        Union();
        Union(const QString &name, Scope *scope);

        SharedField getField(const QString &name);
        SharedField addField(const QString &name, ExtendedType *type);
        void removeField(const QString &name);
        bool containsField(const QString &name);

        FieldsList fields() const;

    private:
        Fields m_Fields;
    };

} // namespace entity
