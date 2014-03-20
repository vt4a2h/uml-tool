#include "scope.h"
#include <utility>

namespace entity {

    Scope::Scope(const QString &scopeName)
        : m_Name(scopeName)
    {
    }

    Scope::Scope(const Scope &src)
        : m_Name(src.m_Name)
    {
    }

    Scope::Scope(Scope &&src)
    {
        m_Name = std::move(src.m_Name);
    }

    Scope::~Scope()
    {
    }

    Scope &Scope::operator =(const Scope &rhs)
    {
        if (this != &rhs) {
            Scope tmp(rhs);
            this->swap(tmp);
        }

        return *this;
    }

    Scope &Scope::operator =(Scope &&rhs)
    {
        if (this != &rhs) {
            m_Name = std::move(rhs.m_Name);
        }

        return *this;
    }

    bool operator ==(const Scope &lhs, const Scope &rhs)
    {
        return lhs.m_Name == rhs.m_Name;
    }

    QString Scope::name() const
    {
        return m_Name;
    }

    void Scope::setName(const QString &name)
    {
        m_Name = name;
    }

    void Scope::swap(Scope &other)
    {
        std::swap(m_Name, other.m_Name);
    }


} // namespace entity
