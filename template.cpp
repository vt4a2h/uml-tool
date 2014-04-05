#include "template.h"
#include "extendedtype.h"
#include "type.h"
#include "constants.cpp"
#include "helpfunctions.h"

#include <algorithm>
#include <utility>

#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

namespace entity {

    TemplateParameter Template::getTemplateParameter(const QString &name) const
    {
        auto it = m_TemplateParameters.find(name);
        return (it != m_TemplateParameters.end() ? *it : std::make_pair(DEFAULT_NAME, STUB_ID));
    }

    void Template::addTemplateParameter(const QString &name, const QString &typeId)
    {
        m_TemplateParameters.insert(name, std::make_pair(name, typeId.isEmpty() ? STUB_ID : typeId));
    }

    bool Template::contains(const QString &name) const
    {
        return m_TemplateParameters.contains(name);
    }

    void Template::removeParameter(const QString &name)
    {
        m_TemplateParameters.remove(name);
    }

    TemplateParametersList Template::parameters() const
    {
        return m_TemplateParameters.values();
    }

    QJsonArray Template::templatePartToJson() const
    {
        QJsonArray result;

        QJsonObject templateParameter;
        for (auto value : m_TemplateParameters.values()) {
            templateParameter.insert("Type name", value.first);
            templateParameter.insert("Default type id", value.second);
            result.append(templateParameter);
        }

        return result;
    }

    void Template::templatePartFromJson(const QJsonArray &src, QStringList &errorList)
    {
        m_TemplateParameters.clear();

        TemplateParameter parameter;
        QJsonObject obj;
        for (auto value : src) {
            obj = value.toObject();
            utility::checkAndSet(obj, "Type name", errorList, [&obj, &parameter, this](){
                parameter.first = obj["Type name"].toString();
            });
            utility::checkAndSet(obj, "Default type id", errorList, [&obj, &parameter, this](){
                parameter.second = obj["Default type id"].toString();
            });
            m_TemplateParameters.insert(parameter.first, parameter);
        }
    }

} // namespace entity
