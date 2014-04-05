#pragma once

#include "class.h"
#include "template.h"

namespace entity {

    class TemplateClass : public Class, public Template
    {
    public:
        TemplateClass();
        TemplateClass(const QString &name, const QString &scopeId);

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;
    };

} // namespace entity
