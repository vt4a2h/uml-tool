#include "templateclassmethod.h"
#include "constants.cpp"
#include "enums.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

#include <utility/helpfunctions.h>

namespace entity {

    /**
     * @brief TemplateClassMethod::TemplateClassMethod
     */
    TemplateClassMethod::TemplateClassMethod()
        : TemplateClassMethod(DEFAULT_NAME)
    {
    }

    /**
     * @brief TemplateClassMethod::TemplateClassMethod
     * @param name
     */
    TemplateClassMethod::TemplateClassMethod(const QString &name)
        : ClassMethod(name)
        , Template()
    {
        m_Type = TemplateMethod;
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const TemplateClassMethod &lhs, const TemplateClassMethod &rhs)
    {
        return static_cast<const ClassMethod&>(lhs).isEqual(rhs) &&
               static_cast<const Template&>(lhs).templatePartEq(rhs);
    }

    /**
     * @brief TemplateClassMethod::toJson
     * @return
     */
    QJsonObject TemplateClassMethod::toJson() const
    {
        QJsonObject result(ClassMethod::toJson());
        result.insert("Template part", Template::templateToJson());

        return result;
    }

    /**
     * @brief TemplateClassMethod::fromJson
     * @param src
     * @param errorList
     */
    void TemplateClassMethod::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        ClassMethod::fromJson(src, errorList);

        utility::checkAndSet(src, "Template part", errorList, [&src, &errorList, this](){
            Template::templateLoadFromJson(src["Template part"].toObject(), errorList);
        });
    }

    /**
     * @brief TemplateClassMethod::isEqual
     * @param rhs
     * @return
     */
    bool TemplateClassMethod::isEqual(const TemplateClassMethod &rhs) const
    {
        return *this == rhs;
    }

} // namespace entity
