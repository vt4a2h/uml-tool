#include "union.h"
#include "field.h"
#include "extendedtype.h"
#include "constants.cpp"
#include "enums.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

#include <utility/helpfunctions.h>

namespace entity {

    Union::Union()
        : Union(DEFAULT_NAME, GLOBAL_SCOPE_ID)
    {
    }

    Union::Union(const Union &src)
        : Type(src)
    {
        copyFrom(src);
    }

    Union::Union(Union &&src)
    {
        moveFrom(src);
    }

    Union::Union(const QString &name, const QString &scopeId)
        : Type(name, scopeId)
    {
        m_KindOfType = UnionType;
    }

    Union &Union::operator=(Union rhs)
    {
        moveFrom(rhs);

        return *this;
    }

    Union &Union::operator=(Union &&rhs)
    {
        if (this != &rhs)
            moveFrom(rhs);

        return *this;
    }

    bool operator ==(const Union &lhs, const Union &rhs)
    {
        return static_cast<const Type&>(lhs).isEqual(rhs) &&
               utility::seqSharedPointerEq(lhs.m_Fields, rhs.m_Fields);
    }

    SharedField Union::getField(const QString &name) const
    {
        auto it = std::find_if(m_Fields.begin(), m_Fields.end(),
                               [&](const SharedField &f) {return f->name() == name;});

        return (it != m_Fields.end() ? *it : nullptr);
    }

    SharedField Union::addField(const QString &name, const QString &typeId)
    {
        auto field = std::make_shared<Field>(name, typeId);

        if (getField(name) != nullptr) removeField(name);
        m_Fields.append(field);

        return field;
    }

    void Union::removeField(const QString &name)
    {
        auto field = getField(name);
        if (field) m_Fields.removeAll(field);
    }

    bool Union::containsField(const QString &name)
    {
        return (getField(name) != nullptr);
    }

    FieldsList Union::fields() const
    {
        return m_Fields;
    }

    QJsonObject Union::toJson() const
    {
        QJsonObject result(Type::toJson());

        QJsonArray fields;
        for (auto &&value : m_Fields) fields.append(value->toJson());
        result.insert("Fields", fields);

        return result;
    }

    void Union::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        Type::fromJson(src, errorList);

        m_Fields.clear();
        utility::checkAndSet(src, "Fields", errorList, [&src, &errorList, this](){
            if (src["Fields"].isArray()) {
                SharedField f;
                for (auto &&value : src["Fields"].toArray()) {
                    f = std::make_shared<Field>();
                    f->fromJson(value.toObject(), errorList);
                    m_Fields.append(f);
                }
            } else {
                errorList << "Error: \"Fields\" is not array";
            }
        });
    }

    Union *Union::clone() const
    {
        return new Union(*this);
    }

    bool Union::isEqual(const Union &rhs) const
    {
        return *this == rhs;
    }

    void Union::moveFrom(Union &src)
    {
        Type::moveFrom(src);
        m_Fields = std::move(src.m_Fields);
    }

    void Union::copyFrom(const Union &src)
    {
        utility::deepCopySharedPointerList(src.m_Fields, m_Fields);
    }

} // namespace entity
