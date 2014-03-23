#pragma once

#include <utility>
#include <QString>
#include <QList>

namespace entity {

    class Scope;
    class Type;

    class ExtendedType
    {
    public:
        using Pl     = std::pair<QString, bool>;
        using PlList = QList<Pl>;
        using ExtendedTypes = QList<ExtendedType*>;

        ExtendedType();
        ExtendedType(Scope *scope, Type *type);

        bool isLink() const;
        void addPointerStatus(bool pointerToConst = false);
        void removePointerStatus();

        bool isPointer() const;
        void addLinkStatus();
        void removeLinkStatus();

        PlList pl() const;

        bool isConst() const;
        void setConstStatus(bool status);

        Scope *scope() const;
        void setScope(Scope *scope);

        Type *type() const;
        void setType(Type *type);

        QString alias() const;
        void setAlias(const QString &alias);

    protected:
        bool    m_ConstStatus;
        Scope  *m_Scope;
        Type   *m_Type;
        QString m_Alias;
        PlList  m_Pl;
    };

} // namespace entity
