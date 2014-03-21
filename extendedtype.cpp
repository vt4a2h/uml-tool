#include "extendedtype.h"
#include "scope.h"
#include "type.h"

namespace entity {

    ExtendedType::ExtendedType()
        : ExtendedType(nullptr, nullptr)
    {
    }

    ExtendedType::ExtendedType(Scope *scope, Type *type)
        : m_ConstStatus(false)
        , m_Scope(scope)
        , m_Type(type)
        , m_Alias("")
    {
    }

    bool ExtendedType::isLink() const
    {
        return m_Pl.last().first == "&";
    }

    void ExtendedType::addPointerStatus(bool pointerToConst)
    {
        m_Pl.append(std::make_pair("*", pointerToConst));
    }

    void ExtendedType::removePointerStatus()
    {
        if (isPointer()) m_Pl.removeLast();
    }

    bool ExtendedType::isPointer() const
    {
        return m_Pl.last().first == "*";
    }

    void ExtendedType::addLinkStatus()
    {
        m_Pl.append(std::make_pair("&", false));
    }

    void ExtendedType::removeLinkStatus()
    {
        if (isLink()) m_Pl.removeLast();
    }

    ExtendedType::PlList ExtendedType::pl() const
    {
        return m_Pl;
    }

    bool ExtendedType::isConst() const
    {
        return m_ConstStatus;
    }

    void ExtendedType::setConstStatus(bool status)
    {
        m_ConstStatus = status;
    }

    Scope *ExtendedType::scope() const
    {
        return m_Scope;
    }
    
    void ExtendedType::setScope(Scope *scope)
    {
        m_Scope = scope;
    }

    Type *ExtendedType::type() const
    {
        return m_Type;
    }

    void ExtendedType::setType(Type *type)
    {
        m_Type = type;
    }

    QString ExtendedType::alias() const
    {
        return m_Alias;
    }

    void ExtendedType::setAlias(const QString &alias)
    {
        m_Alias = alias;
    }

    
} // namespace entity
