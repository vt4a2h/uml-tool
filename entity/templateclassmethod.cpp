#include "templateclassmethod.h"
#include "constants.cpp"
#include "enums.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

#include <utility/helpfunctions.h>

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

    bool operator ==(const TemplateClassMethod &lhs, const TemplateClassMethod &rhs)
    {
        return static_cast<const ClassMethod&>(lhs).isEqual(rhs) &&
               static_cast<const Template&>(lhs).templatePartEq(rhs);
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

    bool TemplateClassMethod::isEqual(const TemplateClassMethod &rhs) const
    {
        return *this == rhs;
    }

} // namespace entity
