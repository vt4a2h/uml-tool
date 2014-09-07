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

    Template::Template()
        : m_LocalDatabase(std::make_shared<db::Database>())
    {
        m_LocalDatabase->addScope();
    }

    TemplateParameter Template::getTemplateParameter(const QString &typeId) const
    {
        auto it = std::find_if(m_TemplateParameters.begin(), m_TemplateParameters.end(),
                               [&typeId](const TemplateParameter &p) { return p.first == typeId; });

        return (it != m_TemplateParameters.end() ? *it : TemplateParameter(STUB_ID, STUB_ID));
    }

    void Template::addTemplateParameter(const QString &typeId, const QString &defaultTypeId)
    {
        if (contains(typeId)) removeParameter(typeId);
        m_TemplateParameters.append(TemplateParameter(typeId,
                                                      defaultTypeId.isEmpty() ? STUB_ID : defaultTypeId));
    }

    bool Template::contains(const QString &typeId) const
    {
        return getTemplateParameter(typeId).first != STUB_ID;
    }

    bool Template::removeParameter(const QString &typeId)
    {
        return m_TemplateParameters.removeOne(getTemplateParameter(typeId));
    }

    TemplateParametersList Template::templateParameters() const
    {
        return m_TemplateParameters;
    }

    const db::SharedDatabase Template::database() const
    {
        return m_LocalDatabase;
    }

    SharedType Template::getLocaleType(const QString &typeId) const
    {
        return m_LocalDatabase->anyScopes() ?
                    m_LocalDatabase->scopes()[0]->getType(typeId) : nullptr;
    }

    bool Template::containsLocaleType(const QString &typeId) const
    {
        return m_LocalDatabase->anyScopes() ?
                    m_LocalDatabase->scopes()[0]->containsType(typeId) : false;
    }

    void Template::removeLocaleType(const QString &typeId)
    {
        if (m_LocalDatabase->anyScopes())
            m_LocalDatabase->scopes()[0]->removeType(typeId);
    }

    TypesList Template::localeTypes() const
    {
        return m_LocalDatabase->anyScopes() ?
                    m_LocalDatabase->scopes()[0]->types() : TypesList();
    }

    QJsonObject Template::templateToJson() const
    {
        QJsonObject result;

        QJsonArray parameters;
        QJsonObject templateParameter;
        for (auto &&value : m_TemplateParameters) {
            templateParameter.insert("Type ID", value.first);
            templateParameter.insert("Default type ID", value.second);
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
                    utility::checkAndSet(obj, "Type ID", errorList, [&obj, &parameter, this](){
                        parameter.first = obj["Type ID"].toString();
                    });
                    utility::checkAndSet(obj, "Default type ID", errorList, [&obj, &parameter, this](){
                        parameter.second = obj["Default type ID"].toString();
                    });
                    m_TemplateParameters.append(parameter);
                }
            } else {
                errorList << "Error: \"Template parameters\" is not array";
            }
        });

        if (!m_LocalDatabase)
            m_LocalDatabase = std::make_shared<db::Database>();
        else
            m_LocalDatabase->clear();

        utility::checkAndSet(src, "Local database", errorList, [&src, &errorList, this](){
            m_LocalDatabase->fromJson(src["Local database"].toObject(), errorList);
        });
    }
    
} // namespace entity
