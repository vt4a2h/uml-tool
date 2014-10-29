#pragma once

#include "class.h"
#include "template.h"

namespace entity {

    /**
     * @brief The TemplateClass class
     */
    class TemplateClass : public Class, public Template
    {
    public:
        TemplateClass();
        TemplateClass(const QString &name, const QString &scopeId);

        friend bool operator ==(const TemplateClass &lhs, const TemplateClass &rhs);

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        bool isEqual(const TemplateClass &rhs) const;
    };

} // namespace entity
