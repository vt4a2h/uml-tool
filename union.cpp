#include "union.h"
#include "field.h"
#include "extendedtype.h"
#include "constants.cpp"
#include "helpfunctions.h"
#include "enums.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

namespace entity {

    Union::Union()
        : Union(DEFAULT_NAME, GLOBAL_SCOPE_ID)
    {
    }

    Union::Union(const QString &name, const QString &scopeId)
        : Type(name, scopeId)
    {
        m_KindOfType = UnionType;
    }

    SharedField Union::getField(const QString &name)
    {
        return m_Fields.value(name);
    }

    SharedField Union::addField(const QString &name, const QString &typeId)
    {
        return *m_Fields.insert(name, std::make_shared<Field>(name, typeId));
    }

    void Union::removeField(const QString &name)
    {
        m_Fields.remove(name);
    }

    bool Union::containsField(const QString &name)
    {
        return m_Fields.contains(name);
    }

    FieldsList Union::fields() const
    {
        return m_Fields.values();
    }

    QJsonObject Union::toJson() const
    {
        QJsonObject result(Type::toJson());

        QJsonArray fields;
        for (auto value : m_Fields.values()) fields.append(value->toJson());
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
                for (auto value : src["Fields"].toArray()) {
                    f = std::make_shared<Field>();
                    f->fromJson(value.toObject(), errorList);
                    m_Fields.insert(f->name(), f);
                }
            } else {
                errorList << "Error: \"Fields\" is not array";
            }
        });
    }

} // namespace entity
