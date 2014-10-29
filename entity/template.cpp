#include "template.h"
#include "extendedtype.h"
#include "type.h"
#include "constants.cpp"

#include <algorithm>
#include <utility>

#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

#include <utility/helpfunctions.h>

namespace entity {

    /**
     * @brief Template::Template
     */
    Template::Template()
        : m_LocalDatabase(std::make_shared<db::Database>())
    {
        m_LocalDatabase->addScope();
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const Template &lhs, const Template &rhs)
    {
        return lhs.m_TemplateParameters == rhs.m_TemplateParameters &&
               (lhs.m_LocalDatabase == rhs.m_LocalDatabase || *lhs.m_LocalDatabase == *rhs.m_LocalDatabase);
    }

    /**
     * @brief Template::getTemplateParameter
     * @param typeId
     * @return
     */
    TemplateParameter Template::getTemplateParameter(const QString &typeId) const
    {
        auto it = std::find_if(m_TemplateParameters.begin(), m_TemplateParameters.end(),
                               [&typeId](const TemplateParameter &p) { return p.first == typeId; });

        return (it != m_TemplateParameters.end() ? *it : TemplateParameter(STUB_ID, STUB_ID));
    }

    /**
     * @brief Template::addTemplateParameter
     * @param typeId
     * @param defaultTypeId
     */
    void Template::addTemplateParameter(const QString &typeId, const QString &defaultTypeId)
    {
        if (contains(typeId)) removeParameter(typeId);
        m_TemplateParameters.append(TemplateParameter(typeId,
                                                      defaultTypeId.isEmpty() ? STUB_ID : defaultTypeId));
    }

    /**
     * @brief Template::contains
     * @param typeId
     * @return
     */
    bool Template::contains(const QString &typeId) const
    {
        return getTemplateParameter(typeId).first != STUB_ID;
    }

    /**
     * @brief Template::removeParameter
     * @param typeId
     * @return
     */
    bool Template::removeParameter(const QString &typeId)
    {
        return m_TemplateParameters.removeOne(getTemplateParameter(typeId));
    }

    /**
     * @brief Template::templateParameters
     * @return
     */
    TemplateParametersList Template::templateParameters() const
    {
        return m_TemplateParameters;
    }

    /**
     * @brief Template::database
     * @return
     */
    const db::SharedDatabase Template::database() const
    {
        return m_LocalDatabase;
    }

    /**
     * @brief Template::getLocaleType
     * @param typeId
     * @return
     */
    SharedType Template::getLocaleType(const QString &typeId) const
    {
        return m_LocalDatabase->anyScopes() ?
                    m_LocalDatabase->scopes()[0]->getType(typeId) : nullptr;
    }

    /**
     * @brief Template::containsLocaleType
     * @param typeId
     * @return
     */
    bool Template::containsLocaleType(const QString &typeId) const
    {
        return m_LocalDatabase->anyScopes() ?
                    m_LocalDatabase->scopes()[0]->containsType(typeId) : false;
    }

    /**
     * @brief Template::removeLocaleType
     * @param typeId
     */
    void Template::removeLocaleType(const QString &typeId)
    {
        if (m_LocalDatabase->anyScopes())
            m_LocalDatabase->scopes()[0]->removeType(typeId);
    }

    /**
     * @brief Template::localeTypes
     * @return
     */
    TypesList Template::localeTypes() const
    {
        return m_LocalDatabase->anyScopes() ?
                    m_LocalDatabase->scopes()[0]->types() : TypesList();
    }

    /**
     * @brief Template::templateToJson
     * @return
     */
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

    /**
     * @brief Template::templateLoadFromJson
     * @param src
     * @param errorList
     */
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

    /**
     * @brief Template::templatePartEq
     * @param rhs
     * @return
     */
    bool Template::templatePartEq(const Template &rhs) const
    {
        return *this == rhs;
    }

} // namespace entity
