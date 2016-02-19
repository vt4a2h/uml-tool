/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 27/06/2015.
**
** This file is part of Q-UML (UML tool for Qt).
**
** Q-UML is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Q-UML is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.

** You should have received a copy of the GNU Lesser General Public License
** along with Q-UML.  If not, see <http://www.gnu.org/licenses/>.
**
*****************************************************************************/
#pragma once

#include <memory>

#include <db/db_types.hpp>

#include "classmethod.h"
#include "basicentity.h"
#include "constants.h"

namespace entity {

    /// The class Property
    class Property : public BasicEntity, public std::enable_shared_from_this<Property>
    {
        Q_OBJECT

    public:
        Property();
        Property(Property &&src);
        Property(const Property &src);
        Property(const QString &name, const EntityID &typeId, QObject *parent = nullptr);

        Property &operator =(const Property &rhs);
        Property &operator =(Property &&rhs);
        friend bool operator ==(const Property &lhs, const Property &rhs);

        Property &setName(const QString &name);
        QString name() const override;

        SharedField field() const;
        Property &addField(const QString &name = "", const EntityID &typeId = EntityID::nullID());
        void deleteField();

        Property &addGetter(const QString &customName = "");
        void deleteGetter();
        SharedMethod getter() const;

        Property &addSetter(const QString &customName = "");
        void deleteSetter();
        SharedMethod setter() const;

        Property &addResetter(const QString &customName = "");
        void deleteResetter();
        SharedMethod resetter() const;

        Property &addNotifier(const QString &customName = "");
        void deleteNotifier();
        SharedMethod notifier() const;

        int revision() const;
        Property &setRevision(int revision);
        bool isRevisionDefault() const;

        bool isDesignable() const;
        bool isDesignableDefault() const;
        Property &addDesignableGetter(const QString &customName = "");
        SharedMethod designableGetter() const;
        void deleteDesignableGetter();
        Property &setDesignable(bool designable);

        bool isScriptable() const;
        bool isScriptableDefault() const;
        Property &addScriptableGetter(const QString &customName = "");
        SharedMethod scriptableGetter() const;
        void deleteScriptableGetter();
        Property &setScriptable(bool scriptable);

        bool isStored() const;
        bool isStoredDefault() const;
        Property &setStored(bool stored);

        bool isUser() const;
        bool isUserDefault() const;
        Property &setUser(bool user);

        bool isConstant() const;
        bool isConstantDefault() const;
        Property &setConstant(bool constant);

        bool isFinal() const;
        bool isFinalDefault() const;
        Property &setFinal(bool final);

        bool isMember() const;
        bool isMemberDefault() const;
        Property &setMember(bool member);

        entity::EntityID typeId() const;
        void setTypeId(const entity::EntityID &typeId);

    public: // BasicEntity implementation
        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        size_t hashType() const override;
        static size_t staticHashType();

        QString marker() const override;
        static QString staticMarker();

    // TODO: connect!
    public slots:
        void setTypeSearcher(const db::SharedTypeSearcher &typeSearcher);

    signals:
        void methodAdded(const entity::SharedProperty &, const entity::SharedMethod &);
        void methodRemoved(const entity::SharedProperty &, const entity::SharedMethod &);

        void fieldAdded(const entity::SharedProperty &, const entity::SharedField &);
        void fieldRemoved(const entity::SharedProperty &, const entity::SharedField &);

    protected:
        void moveFrom(Property &&src);
        void copyFrom(const Property &src);

    private:
        void init();
        SharedProperty safeShared();

        // TODO: add the same for field!
        template <class Method>
        void assignMethod(SharedMethod &dst, Method src)
        {
            if (dst)
                emit methodRemoved(safeShared(), dst);

            dst = std::forward<Method>(src);

            if (dst)
                emit methodAdded(safeShared(), dst);
        }

    private:
        db::SharedTypeSearcher typeSearcher() const;

        db::WeakTypeSearcher m_typeSearcher;

        SharedField m_Field;

        SharedMethod m_Getter;
        SharedMethod m_Setter;
        SharedMethod m_Resetter;
        SharedMethod m_Notifier;

        SharedMethod m_DesignableGetter;
        SharedMethod m_ScriptableGetter;

        int m_Revision;

        bool m_Member;
        bool m_Designable;
        bool m_Scriptable;
        bool m_Stored;
        bool m_User;
        bool m_Constant;
        bool m_Final;
    };

} // namespace entity
