#pragma once

#include <QString>
#include "types.h"

namespace entity {

    class Template
    {
    public:
        TemplateParameter getTemplateParameter(const QString &typeId) const;
        void addTemplateParameter(const QString &typeId, const QString &defaultTypeId= QString(""));
        bool contains(const QString &typeId) const;
        bool removeParameter(const QString &typeId);
        TemplateParametersList parameters() const;

        db::SharedDatabase localDatabase() const;
        void setLocalDatabase(const db::SharedDatabase &localDatabase);

        QJsonObject templateToJson() const;
        void templateLoadFromJson(const QJsonObject &src, QStringList &errorList);

    private:
        TemplateParametersList m_TemplateParameters;
        db::SharedDatabase m_LocalDatabase;
    };

} // namespace entity

