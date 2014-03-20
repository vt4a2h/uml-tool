#include "extendedtype.h"

namespace entity {

    ExtendedType::ExtendedType()
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

    void ExtendedType::setAlias(const QString &name)
    {
        m_Alias = name;
    }

    int ExtendedType::typeId() const
    {
        return m_TypeId;
    }

    void ExtendedType::setTypeId(int typeId)
    {
        m_TypeId = typeId;
    }

} // namespace entity
