#include "templateclassmethod.h"
#include "constants.cpp"
#include "enums.h"
#include "helpfunctions.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

namespace entity {

    TemplateClassMethod::TemplateClassMethod()
        : TemplateClassMethod(DEFAULT_NAME)
    {
    }

    TemplateClassMethod::TemplateClassMethod(const QString &name)
        : ClassMethod(name)
        , Template()
    {
        m_Type = TemplateMethod;
    }

    QJsonObject TemplateClassMethod::toJson() const
    {
        QJsonObject result(ClassMethod::toJson());
        result.insert("Template part", Template::templateToJson());

        return result;
    }

    void TemplateClassMethod::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        ClassMethod::fromJson(src, errorList);

        utility::checkAndSet(src, "Template part", errorList, [&src, &errorList, this](){
            Template::templateLoadFromJson(src["Template part"].toObject(), errorList);
        });
    }

} // namespace entity
