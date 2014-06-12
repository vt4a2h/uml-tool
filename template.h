#pragma once

#include <QString>
#include "types.h"

namespace entity {

    class Template
    {
    public:
        TemplateParameter getTemplateParameter(const QString &name) const;
        void addTemplateParameter(const QString &name, const QString &typeId = QString(""));
        bool contains(const QString &name) const;
        void removeParameter(const QString &name);
        TemplateParametersList parameters() const;

        db::SharedDatabase localDatabase() const;
        void setLocalDatabase(const db::SharedDatabase &localDatabase);

        QJsonObject templateToJson() const;
        void templateLoadFromJson(const QJsonObject &src, QStringList &errorList);

    private:
        TemplateParameters m_TemplateParameters;
        db::SharedDatabase m_LocalDatabase;
    };

} // namespace entity

