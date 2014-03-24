#pragma once

#include <QString>
#include <QHash>
#include <memory>

namespace entity {

    class Scope;
    class Field;

    class Type
    {
    public:
        using SharedField = std::shared_ptr<Field>;
        using Fields = QHash<QString, SharedField>;

        Type();
        Type(const QString &name, Scope *scope);

        QString name() const;
        void setName(const QString &name);

        Scope *scope() const;
        void setScope(Scope *scope);

    private:
        QString m_Name;
        Scope  *m_Scope;
    };

} // namespace entity
