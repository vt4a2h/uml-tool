#include "templateclass.h"
#include "constants.cpp"
#include "enums.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

#include <utility/helpfunctions.h>

namespace entity {

    /**
     * @brief TemplateClass::TemplateClass
     */
    TemplateClass::TemplateClass()
        : TemplateClass(DEFAULT_NAME, GLOBAL_SCOPE_ID)
    {
    }

    /**
     * @brief TemplateClass::TemplateClass
     * @param name
     * @param scopeId
     */
    TemplateClass::TemplateClass(const QString &name, const QString &scopeId)
        : Class (name, scopeId)
        , Template()
    {
        m_KindOfType = TemplateClassType;
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const TemplateClass &lhs, const TemplateClass &rhs)
    {
        return static_cast<const Class&>(lhs).isEqual(rhs) &&
               static_cast<const Template&>(lhs).templatePartEq(rhs);
    }

    /**
     * @brief TemplateClass::toJson
     * @return
     */
    QJsonObject TemplateClass::toJson() const
    {
        QJsonObject result(Class::toJson());
        result.insert("Template part", Template::templateToJson());

        return result;
    }

    /**
     * @brief TemplateClass::fromJson
     * @param src
     * @param errorList
     */
    void TemplateClass::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        Class::fromJson(src, errorList);

        utility::checkAndSet(src, "Template part", errorList, [&src, &errorList, this](){
            Template::templateLoadFromJson(src["Template part"].toObject(), errorList);
        });
    }

    /**
     * @brief TemplateClass::isEqual
     * @param rhs
     * @return
     */
    bool TemplateClass::isEqual(const TemplateClass &rhs) const
    {
        return *this == rhs;
    }

} // namespace entity
