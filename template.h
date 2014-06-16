#pragma once

#include <QString>
#include "types.h"
#include "database.h"
#include "scope.h"

namespace entity {

    class Template
    {
    public:
        Template();
        TemplateParameter getTemplateParameter(const QString &typeId) const;
        void addTemplateParameter(const QString &typeId, const QString &defaultTypeId= QString(""));
        bool contains(const QString &typeId) const;
        bool removeParameter(const QString &typeId);
        TemplateParametersList parameters() const;

        const db::SharedDatabase database() const;
        SharedType getLocaleType(const QString &typeId) const;
        template <class T = Type> std::shared_ptr<T> addLocaleType(const QString &name = "");
        bool containsLocaleType(const QString &typeId) const;
        void removeLocaleType(const QString &typeId);
        TypesList localeTypes() const;

        QJsonObject templateToJson() const;
        void templateLoadFromJson(const QJsonObject &src, QStringList &errorList);

    private:
        TemplateParametersList m_TemplateParameters;
        db::SharedDatabase m_LocalDatabase;
    };

    template <class T>
    std::shared_ptr<T> Template::addLocaleType(const QString &name)
    {
        auto scope = m_LocalDatabase->getScope("_global_scope");
        return (scope ? scope->addType<T>(name) : nullptr);
    }

} // namespace entity

