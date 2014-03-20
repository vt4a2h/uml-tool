#pragma once

#include <QString>

namespace entity {

    class Scope
    {
    public:
        Scope(const QString &scopeName = "");
        Scope(const Scope &src);
        Scope(Scope &&src);
        virtual ~Scope();

        friend bool operator ==(const Scope &lhs, const Scope &rhs);
        Scope &operator =(const Scope &rhs);
        Scope &operator =(Scope &&rhs);

        QString name() const;
        void setName(const QString &name);

    private:
        void swap(Scope &other);

        QString m_Name;
    };

} // namespace entity
