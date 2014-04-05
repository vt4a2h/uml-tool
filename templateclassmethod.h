#pragma once

#include "classmethod.h"
#include "template.h"

namespace entity {

    class TemplateClassMethod : public ClassMethod, public Template
    {
    public:
        TemplateClassMethod();
        TemplateClassMethod(const QString &name);

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;
    };

} // namespace entity
