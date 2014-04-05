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

    protected:
        QJsonArray templatePartToJson() const;
        void templatePartFromJson(const QJsonArray &src, QStringList &errorList);

    private:
        TemplateParameters m_TemplateParameters;
    };

} // namespace entity

