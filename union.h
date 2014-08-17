#pragma once

#include "type.h"
#include "types.h"

namespace entity {

    class Field;

    class Union : public Type
    {
    public:
        Union();
        Union(Union &&src);
        Union(const Union &src);
        Union(const QString &name, const QString &scopeId);

        Union &operator= (Union rhs);
        Union &operator= (Union &&rhs);

        SharedField getField(const QString &name) const;
        SharedField addField(const QString &name, const QString &typeId);
        void removeField(const QString &name);
        bool containsField(const QString &name);

        FieldsList fields() const;

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        Union *clone() const;

    private:
        void moveFrom(Union &src);
        void copy(const Union &src);

        FieldsList m_Fields;
    };

} // namespace entity
