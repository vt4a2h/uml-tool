#include "templateclass.h"
#include "constants.cpp"
#include "enums.h"
#include "helpfunctions.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

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

    QJsonObject TemplateClass::toJson() const
    {
        QJsonObject result(Class::toJson());
        result.insert("Template parameters", templatePartToJson());

        return result;
    }

    void TemplateClass::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        Class::fromJson(src, errorList);

        utility::checkAndSet(src, "Template parameters", errorList, [&src, &errorList, this](){
            if (src["Template parameters"].isArray()) {
                templatePartFromJson(src["Template parameters"].toArray(), errorList);
            } else {
                errorList << "Error: \"Template parameters\" is not array";
            }
        });
    }

} // namespace entity
