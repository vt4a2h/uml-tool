#pragma once

#include <utility>
#include <QString>
#include <QList>

namespace entity {

    class ExtendedType
    {
    public:
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

        QString alias() const;
        void setAlias(const QString &alias);

        int typeId() const;
        void setTypeId(int typeId);

    protected:
        using Pl     = std::pair<QString, bool>;
        using PlList = QList<Pl>;

        bool    m_ConstStatus;
        int     m_TypeId;
        QString m_Alias;
        PlList  m_Pl;
    };

} // namespace entity
