#include "extendedtype.h"
#include "scope.h"
#include "type.h"
#include "helpfunctions.h"

namespace entity {

    ExtendedType::ExtendedType()
        : ExtendedType(nullptr, nullptr)
    {
    }

    ExtendedType::ExtendedType(Scope *scope, Type *type, const QString &alias)
        : m_ConstStatus(false)
        , m_Scope(scope)
        , m_Type(type)
        , m_Alias(alias)
        , m_Id(utility::genId())
    {
    }

    ExtendedType::~ExtendedType()
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

    void ExtendedType::addTemplateParameter(ExtendedType *t)
    {
        m_TemplateParameters << t;
    }

    bool ExtendedType::containsTemplateParameter(ExtendedType *t) const
    {
        return m_TemplateParameters.contains(t);
    }

    void ExtendedType::removeTemplateParameters(ExtendedType *t)
    {
        m_TemplateParameters.removeAll(t);
    }

    ExtendedTypesRawList ExtendedType::templateParameters() const
    {
        return m_TemplateParameters;
    }

    QString ExtendedType::id() const
    {
        return m_Id;
    }
    
    void ExtendedType::setId(const QString &id)
    {
        m_Id = id;
    }
    
} // namespace entity
