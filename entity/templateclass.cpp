#include "templateclass.h"
#include "constants.cpp"
#include "enums.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

#include <utility/helpfunctions.h>

namespace entity {

    TemplateClass::TemplateClass()
        : TemplateClass(DEFAULT_NAME, GLOBAL_SCOPE_ID)
    {
    }

    TemplateClass::TemplateClass(const QString &name, const QString &scopeId)
        : Class (name, scopeId)
        , Template()
    {
        m_KindOfType = TemplateClassType;
    }

    bool operator ==(const TemplateClass &lhs, const TemplateClass &rhs)
    {
        return static_cast<const Class&>(lhs).isEqual(rhs) &&
               static_cast<const Template&>(lhs).templatePartEq(rhs);
    }

    QJsonObject TemplateClass::toJson() const
    {
        QJsonObject result(Class::toJson());
        result.insert("Template part", Template::templateToJson());

        return result;
    }

    void TemplateClass::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        Class::fromJson(src, errorList);

        utility::checkAndSet(src, "Template part", errorList, [&src, &errorList, this](){
            Template::templateLoadFromJson(src["Template part"].toObject(), errorList);
        });
    }

    bool TemplateClass::isEqual(const TemplateClass &rhs) const
    {
        return *this == rhs;
    }

} // namespace entity
