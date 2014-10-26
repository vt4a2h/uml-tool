#pragma once

#include "classmethod.h"
#include "template.h"

namespace entity {

    class TemplateClassMethod : public ClassMethod, public Template
    {
    public:
        TemplateClassMethod();
        TemplateClassMethod(const QString &name);

        friend bool operator ==(const TemplateClassMethod &lhs, const TemplateClassMethod &rhs);

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        bool isEqual(const TemplateClassMethod &rhs) const;
    };

} // namespace entity
