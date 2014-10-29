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

    /**
     * @brief Union::Union
     */
    Union::Union()
        : Union(DEFAULT_NAME, GLOBAL_SCOPE_ID)
    {
    }

    /**
     * @brief Union::Union
     * @param src
     */
    Union::Union(const Union &src)
        : Type(src)
    {
        copyFrom(src);
    }

    /**
     * @brief Union::Union
     * @param src
     */
    Union::Union(Union &&src)
    {
        moveFrom(src);
    }

    /**
     * @brief Union::Union
     * @param name
     * @param scopeId
     */
    Union::Union(const QString &name, const QString &scopeId)
        : Type(name, scopeId)
    {
        m_KindOfType = UnionType;
    }

    /**
     * @brief Union::operator =
     * @param rhs
     * @return
     */
    Union &Union::operator=(Union rhs)
    {
        moveFrom(rhs);

        return *this;
    }

    /**
     * @brief Union::operator =
     * @param rhs
     * @return
     */
    Union &Union::operator=(Union &&rhs)
    {
        if (this != &rhs)
            moveFrom(rhs);

        return *this;
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const Union &lhs, const Union &rhs)
    {
        return static_cast<const Type&>(lhs).isEqual(rhs) &&
               utility::seqSharedPointerEq(lhs.m_Fields, rhs.m_Fields);
    }

    /**
     * @brief Union::getField
     * @param name
     * @return
     */
    SharedField Union::getField(const QString &name) const
    {
        auto it = std::find_if(m_Fields.begin(), m_Fields.end(),
                               [&](const SharedField &f) {return f->name() == name;});

        return (it != m_Fields.end() ? *it : nullptr);
    }

    /**
     * @brief Union::addField
     * @param name
     * @param typeId
     * @return
     */
    SharedField Union::addField(const QString &name, const QString &typeId)
    {
        auto field = std::make_shared<Field>(name, typeId);

        if (getField(name) != nullptr) removeField(name);
        m_Fields.append(field);

        return field;
    }

    /**
     * @brief Union::removeField
     * @param name
     */
    void Union::removeField(const QString &name)
    {
        auto field = getField(name);
        if (field) m_Fields.removeAll(field);
    }

    /**
     * @brief Union::containsField
     * @param name
     * @return
     */
    bool Union::containsField(const QString &name)
    {
        return (getField(name) != nullptr);
    }

    /**
     * @brief Union::fields
     * @return
     */
    FieldsList Union::fields() const
    {
        return m_Fields;
    }

    /**
     * @brief Union::toJson
     * @return
     */
    QJsonObject Union::toJson() const
    {
        QJsonObject result(Type::toJson());

        QJsonArray fields;
        for (auto &&value : m_Fields) fields.append(value->toJson());
        result.insert("Fields", fields);

        return result;
    }

    /**
     * @brief Union::fromJson
     * @param src
     * @param errorList
     */
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

    /**
     * @brief Union::clone
     * @return
     */
    Union *Union::clone() const
    {
        return new Union(*this);
    }

    /**
     * @brief Union::isEqual
     * @param rhs
     * @return
     */
    bool Union::isEqual(const Union &rhs) const
    {
        return *this == rhs;
    }

    /**
     * @brief Union::moveFrom
     * @param src
     */
    void Union::moveFrom(Union &src)
    {
        Type::moveFrom(src);
        m_Fields = std::move(src.m_Fields);
    }

    /**
     * @brief Union::copyFrom
     * @param src
     */
    void Union::copyFrom(const Union &src)
    {
        utility::deepCopySharedPointerList(src.m_Fields, m_Fields);
    }

} // namespace entity
