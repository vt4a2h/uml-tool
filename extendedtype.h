#pragma once

#include <utility>
#include <QString>
#include <QList>

namespace entity {

    class ExtendedType
    {
    public:
        using Pl     = std::pair<QString, bool>;
        using PlList = QList<Pl>;

        ExtendedType();

        bool isLink() const;
        void addPointerStatus(bool pointerToConst = false);
        void removePointerStatus();

        bool isPointer() const;
        void addLinkStatus();
        void removeLinkStatus();

        PlList pl() const;

        bool isConst() const;
        void setConstStatus(bool status);

        int typeId() const;
        void setTypeId(int typeId);

    protected:
        bool    m_ConstStatus;
        int     m_TypeId;
        PlList  m_Pl;
    };

} // namespace entity
