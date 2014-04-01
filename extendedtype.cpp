#include "extendedtype.h"
#include "scope.h"
#include "type.h"
#include "helpfunctions.h"
#include "constants.cpp"

namespace entity {

    ExtendedType::ExtendedType()
        : ExtendedType(GLOBAL_SCOPE_ID, STUB_ID)
    {
    }

    ExtendedType::ExtendedType(const QString &scopeId, const QString &typeId, const QString &alias)
        : m_ConstStatus(false)
        , m_ScopeId(scopeId)
        , m_TypeId(typeId)
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

    QString ExtendedType::alias() const
    {
        return m_Alias;
    }

    void ExtendedType::setAlias(const QString &alias)
    {
        m_Alias = alias;
    }

    void ExtendedType::addTemplateParameter(const QString &typeId)
    {
        m_TemplateParameters << typeId;
    }

    bool ExtendedType::containsTemplateParameter(const QString &typeId) const
    {
        return m_TemplateParameters.contains(typeId);
    }

    void ExtendedType::removeTemplateParameters(const QString &typeId)
    {
        m_TemplateParameters.removeAll(typeId);
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

    QString ExtendedType::scopeId() const
    {
        return m_ScopeId;
    }

    void ExtendedType::setScopeId(const QString &scopeId)
    {
        m_ScopeId = scopeId;
    }

    QString ExtendedType::typeId() const
    {
        return m_TypeId;
    }

    void ExtendedType::setTypeId(const QString &typeId)
    {
        m_TypeId = typeId;
    }
    
} // namespace entity
