#include "template.h"
#include "extendedtype.h"
#include "type.h"
#include "constants.cpp"
#include "helpfunctions.h"
#include "database.h"

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

    QJsonObject Template::templateToJson() const
    {
        QJsonObject result;

        QJsonArray parameters;
        QJsonObject templateParameter;
        for (auto &&value : m_TemplateParameters.values()) {
            templateParameter.insert("Type name", value.first);
            templateParameter.insert("Default type id", value.second);
            parameters.append(templateParameter);
        }
        result.insert("Template parameters", parameters);

        result.insert("Local database", m_LocalDatabase->toJson());

        return result;
    }

    void Template::templateLoadFromJson(const QJsonObject &src, QStringList &errorList)
    {
        m_TemplateParameters.clear();

        utility::checkAndSet(src, "Template parameters", errorList, [&src, &errorList, this](){
            TemplateParameter parameter;
            QJsonObject obj;
            if (src["Template parameters"].isArray()) {
                for (auto &&value : src["Template parameters"].toArray()) {
                    obj = value.toObject();
                    utility::checkAndSet(obj, "Type name", errorList, [&obj, &parameter, this](){
                        parameter.first = obj["Type name"].toString();
                    });
                    utility::checkAndSet(obj, "Default type id", errorList, [&obj, &parameter, this](){
                        parameter.second = obj["Default type id"].toString();
                    });
                    m_TemplateParameters.insert(parameter.first, parameter);
                }
            } else {
                errorList << "Error: \"Template parameters\" is not array";
            }
        });

        m_LocalDatabase->clear();

        utility::checkAndSet(src, "Local database", errorList, [&src, &errorList, this](){
            m_LocalDatabase->fromJson(src["Local database"].toObject(), errorList);
        });
    }

    db::SharedDatabase Template::localDatabase() const
    {
        return m_LocalDatabase;
    }
    
    void Template::setLocalDatabase(const db::SharedDatabase &localDatabase)
    {
        m_LocalDatabase = localDatabase;
    }   
    
} // namespace entity
