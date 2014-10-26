#pragma once

#include <QString>
#include "types.h"
#include "scope.h"

#include <db/database.h>

namespace entity {

    // TODO: add existing type posibilities
    class Template
    {
    public:
        Template();

        friend bool operator ==(const Template &lhs, const Template &rhs);

        TemplateParameter getTemplateParameter(const QString &typeId) const;
        void addTemplateParameter(const QString &typeId, const QString &defaultTypeId= QString(""));
        bool contains(const QString &typeId) const;
        bool removeParameter(const QString &typeId);
        TemplateParametersList templateParameters() const;

        const db::SharedDatabase database() const;
        SharedType getLocaleType(const QString &typeId) const;
        template <class T = Type> std::shared_ptr<T> addLocaleType(const QString &name = "");
        bool containsLocaleType(const QString &typeId) const;
        void removeLocaleType(const QString &typeId);
        TypesList localeTypes() const;

        QJsonObject templateToJson() const;
        void templateLoadFromJson(const QJsonObject &src, QStringList &errorList);

        bool templatePartEq(const Template &rhs) const;

    private:
        TemplateParametersList m_TemplateParameters;
        db::SharedDatabase m_LocalDatabase;
    };

    template <class T>
    std::shared_ptr<T> Template::addLocaleType(const QString &name)
    {
        return m_LocalDatabase->anyScopes() ?
                    m_LocalDatabase->scopes()[0]->addType<T>(name) : nullptr;
    }

} // namespace entity

