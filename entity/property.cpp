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
#include "property.h"

#include <typeinfo>

#include <QJsonObject>

#include <utility/helpfunctions.h>

#include "field.h"
#include "classmethod.h"

#include "constants.h"

namespace {
    const QString marker = "property";

    const int defaultRevision = 0;

    const bool defaultDesignable = true ;
    const bool defaultScriptable = true ;
    const bool defaultStored     = true ;
    const bool defaultUser       = false;
    const bool defaultConstant   = false;
    const bool defaultFinal      = false;

    const QString nameMark = "Name";
    const QString idMark = "ID";

    const QString fieldMark = "Field";

    const QString getterMark = "Getter";
    const QString setterMark = "Setter";
    const QString resetterMark = "Resetter";
    const QString notifierMark = "Notifier";

    const QString designableGetterMark = "DesignableGetter";
    const QString scriptableGetterMark = "ScriptableGetter";

    const QString revisionMark = "revision";

    const QString designableMark = "Designable";
    const QString scriptableMark = "Scriptable";
    const QString storedMark = "Stored";
    const QString userMark = "User";
    const QString constantMark = "Constant";
    const QString finalMark = "Final";

    void readOptionalMethod(const QJsonValue &val, entity::SharedMethod &dst, ErrorList &errors)
    {
        if (val.isObject()) {
            if (!dst)
                dst = std::make_shared<entity::ClassMethod>();

            dst->fromJson(val.toObject(), errors);
        } else {
            dst.reset();
        }
    }
}

namespace entity {

    /**
     * @brief Property::Property
     */
    Property::Property()
        : Property(DEFAULT_NAME, STUB_ID)
    {
    }

    /**
     * @brief Property::Property
     * @param src
     */
    Property::Property(Property &&src)
        : BasicEntity(std::forward<Property>(src))
    {
        moveFrom(std::forward<Property>(src));
    }

    /**
     * @brief Property::Property
     * @param src
     */
    Property::Property(const Property &src)
        : BasicEntity(src)
    {
        copyFrom(src);
    }

    /**
     * @brief Property::Property
     * @param name
     * @param parent
     */
    Property::Property(const QString &name, const QString &typeId, QObject *parent)
        : BasicEntity(name)
        , m_Id(utility::genId())
        , m_Field(std::make_shared<entity::Field>(name, typeId))
        , m_Revision(defaultRevision)
        , m_Designable(defaultDesignable)
        , m_Scriptable(defaultScriptable)
        , m_Stored(defaultStored)
        , m_User(defaultUser)
        , m_Constant(defaultConstant)
        , m_Final(defaultFinal)
    {
        setParent(parent);
    }

    /**
     * @brief Property::operator =
     * @param rhs
     * @return
     */
    Property &Property::operator =(const Property &rhs)
    {
        if (this != &rhs) {
            BasicEntity::operator =(rhs);
            copyFrom(rhs);
        }

        return *this;
    }

    /**
     * @brief Property::operator =
     * @param rhs
     * @return
     */
    Property &Property::operator =(Property &&rhs)
    {
        if (this != &rhs) {
            BasicEntity::operator =(std::forward<Property>(rhs));
            moveFrom(std::forward<Property>(rhs));
        }

        return *this;
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const Property &lhs, const Property &rhs)
    {
        using namespace utility;

        return lhs.m_Name == rhs.m_Name &&
               lhs.m_Id == rhs.m_Id &&

               sharedPtrEq(lhs.m_Field, rhs.m_Field) &&

               sharedPtrEq(lhs.m_Getter, rhs.m_Getter) &&
               sharedPtrEq(lhs.m_Setter, rhs.m_Setter) &&
               sharedPtrEq(lhs.m_Resetter, rhs.m_Resetter) &&
               sharedPtrEq(lhs.m_Notifier, rhs.m_Notifier) &&

               sharedPtrEq(lhs.m_DesignableGetter, rhs.m_DesignableGetter) &&
               sharedPtrEq(lhs.m_ScriptableGetter, rhs.m_ScriptableGetter) &&

               lhs.m_Revision == rhs.m_Revision &&

               lhs.m_Designable == rhs.m_Designable &&
               lhs.m_Scriptable == rhs.m_Scriptable &&
               lhs.m_Stored     == rhs.m_Stored &&
               lhs.m_User       == rhs.m_User &&
               lhs.m_Constant   == rhs.m_Constant &&
               lhs.m_Final      == rhs.m_Final;
    }

    /**
     * @brief Property::field
     * @return
     */
    SharedField Property::field() const
    {
        return m_Field;
    }

    /**
     * @brief Property::addGetter
     * @param customName
     * @return
     */
    Property &Property::addGetter(const QString &customName)
    {
        QString newName(customName);
        if (newName.isEmpty()) {
            Q_ASSERT(!m_Name.isEmpty());

            newName = m_Name;
            newName[0] = newName[0].toLower();
        }

        m_Getter = std::make_shared<ClassMethod>(newName);

        return *this;
    }

    /**
     * @brief Property::deleteGetter
     * @return
     */
    void Property::deleteGetter()
    {
        m_Getter.reset();
    }

    /**
     * @brief Property::getter
     * @return
     */
    SharedMethod Property::getter() const
    {
        return m_Getter;
    }

    /**
     * @brief Property::addSetter
     * @param customName
     * @return
     */
    Property &Property::addSetter(const QString &customName)
    {
        QString newName(customName);
        if (newName.isEmpty()) {
            Q_ASSERT(!m_Name.isEmpty());

            newName = m_Name;
            newName[0] = newName[0].toUpper();
            newName.prepend("set");
        }

        m_Setter = std::make_shared<ClassMethod>(newName);

        return *this;
    }

    /**
     * @brief Property::deleteSetter
     */
    void Property::deleteSetter()
    {
        m_Setter.reset();
    }

    /**
     * @brief Property::setter
     * @return
     */
    SharedMethod Property::setter() const
    {
        return m_Setter;
    }

    /**
     * @brief Property::addResetter
     * @param customName
     * @return
     */
    Property &Property::addResetter(const QString &customName)
    {
        QString newName(customName);
        if (newName.isEmpty()) {
            Q_ASSERT(!m_Name.isEmpty());

            newName = m_Name;
            newName[0] = newName[0].toUpper();
            newName.prepend("reset");
        }

        m_Resetter = std::make_shared<ClassMethod>(newName);

        return *this;
    }

    /**
     * @brief Property::deleteResetter
     */
    void Property::deleteResetter()
    {
        m_Resetter.reset();
    }

    /**
     * @brief Property::resetter
     * @return
     */
    SharedMethod Property::resetter() const
    {
        return m_Resetter;
    }

    /**
     * @brief Property::addNotifier
     * @param customName
     * @return
     */
    Property &Property::addNotifier(const QString &customName)
    {
        QString newName(customName);
        if (newName.isEmpty()) {
            Q_ASSERT(!m_Name.isEmpty());

            newName = m_Name;
            newName[0] = newName[0].toLower();
            newName.append("Changed");
        }

        m_Notifier = std::make_shared<ClassMethod>(newName);

        return *this;
    }

    /**
     * @brief Property::deleteNotifier
     */
    void Property::deleteNotifier()
    {
        m_Notifier.reset();
    }

    /**
     * @brief Property::notifier
     * @return
     */
    SharedMethod Property::notifier() const
    {
        return m_Notifier;
    }

    /**
     * @brief Property::revision
     * @return
     */
    int Property::revision() const
    {
        return m_Revision;
    }

    /**
     * @brief Property::setRevision
     * @param revision
     * @return
     */
    Property &Property::setRevision(int revision)
    {
        m_Revision = revision;
        return *this;
    }

    /**
     * @brief Property::isRevisionDefault
     * @return
     */
    bool Property::isRevisionDefault() const
    {
        return m_Revision == defaultRevision;
    }

    /**
     * @brief Property::isDesignable
     * @return
     */
    bool Property::isDesignable() const
    {
        return m_Designable;
    }

    /**
     * @brief Property::isDesignableDefault
     * @return
     */
    bool Property::isDesignableDefault() const
    {
        return m_Designable == defaultDesignable;
    }

    /**
     * @brief Property::addDesignableGetter
     * @param customName
     * @return
     */
    Property &Property::addDesignableGetter(const QString &customName)
    {
        QString newName(customName);
        if (newName.isEmpty()) {
            Q_ASSERT(!m_Name.isEmpty());

            newName = m_Name;
            newName[0] = newName[0].toLower();
            newName.append("Designable");
        }

        m_DesignableGetter = std::make_shared<ClassMethod>(newName);
        return *this;
    }

    /**
     * @brief Property::designableGetter
     * @return
     */
    SharedMethod Property::designableGetter() const
    {
        return m_DesignableGetter;
    }

    /**
     * @brief Property::deleteDesignableGetter
     */
    void Property::deleteDesignableGetter()
    {
        m_DesignableGetter.reset();
    }

    /**
     * @brief Property::setDesignable
     * @param designable
     * @return
     */
    Property &Property::setDesignable(bool designable)
    {
        m_Designable = designable;
        return *this;
    }

    /**
     * @brief Property::isScriptable
     * @return
     */
    bool Property::isScriptable() const
    {
        return m_Scriptable;
    }

    /**
     * @brief Property::isScriptableDefault
     * @return
     */
    bool Property::isScriptableDefault() const
    {
        return m_Scriptable == defaultScriptable;
    }

    /**
     * @brief Property::addScriptableGetter
     * @param customName
     * @return
     */
    Property &Property::addScriptableGetter(const QString &customName)
    {
        QString newName(customName);
        if (newName.isEmpty()) {
            Q_ASSERT(!m_Name.isEmpty());

            newName = m_Name;
            newName[0] = newName[0].toLower();
            newName.append("Scriptable");
        }

        m_ScriptableGetter = std::make_shared<ClassMethod>(newName);
        return *this;
    }

    /**
     * @brief Property::ScriptableGetter
     * @return
     */
    SharedMethod Property::scriptableGetter() const
    {
        return m_ScriptableGetter;
    }

    /**
     * @brief Property::deleteScriptableGetter
     */
    void Property::deleteScriptableGetter()
    {
        m_ScriptableGetter.reset();
    }

    /**
     * @brief Property::setScriptable
     * @param Scriptable
     * @return
     */
    Property &Property::setScriptable(bool scriptable)
    {
        m_Scriptable = scriptable;
        return *this;
    }

    /**
     * @brief Property::isStored
     * @return
     */
    bool Property::isStored() const
    {
        return m_Stored;
    }

    /**
     * @brief Property::isStoredDefault
     * @return
     */
    bool Property::isStoredDefault() const
    {
        return m_Stored == defaultStored;
    }

    /**
     * @brief Property::setStored
     * @param stored
     * @return
     */
    Property &Property::setStored(bool stored)
    {
        m_Stored = stored;
        return *this;
    }

    /**
     * @brief Property::isUser
     * @return
     */
    bool Property::isUser() const
    {
        return m_User;
    }

    /**
     * @brief Property::isUserDefault
     * @return
     */
    bool Property::isUserDefault() const
    {
        return m_User == defaultUser;
    }

    /**
     * @brief Property::setUser
     * @param user
     * @return
     */
    Property &Property::setUser(bool user)
    {
        m_User = user;
        return *this;
    }

    /**
     * @brief Property::isConstant
     * @return
     */
    bool Property::isConstant() const
    {
        return m_Constant;
    }

    /**
     * @brief Property::isConstantDefault
     * @return
     */
    bool Property::isConstantDefault() const
    {
        return m_Constant == defaultConstant;
    }

    /**
     * @brief Property::setConstant
     * @param constant
     * @return
     */
    Property &Property::setConstant(bool constant)
    {
        m_Constant = constant;
        return *this;
    }

    /**
     * @brief Property::isFinal
     * @return
     */
    bool Property::isFinal() const
    {
        return m_Final;
    }

    /**
     * @brief Property::isFinalDefault
     * @return
     */
    bool Property::isFinalDefault() const
    {
        return m_Final == defaultFinal;
    }

    /**
     * @brief Property::setFinal
     * @param final
     * @return
     */
    Property &Property::setFinal(bool final)
    {
        m_Final = final;
        return *this;
    }

    /**
     * @brief Property::toJson
     * @return
     */
    QJsonObject Property::toJson() const
    {
        QJsonObject result;

        result.insert(nameMark, m_Name);
        result.insert(idMark, m_Id);
        result.insert(fieldMark, m_Field->toJson());


        result.insert(getterMark, m_Getter ? m_Getter->toJson() : QJsonValue(QString("")));
        result.insert(setterMark, m_Setter ? m_Setter->toJson() : QJsonValue(QString("")));
        result.insert(resetterMark, m_Resetter ? m_Resetter->toJson() : QJsonValue(QString("")));
        result.insert(notifierMark, m_Notifier ? m_Notifier->toJson() : QJsonValue(QString("")));

        result.insert(designableGetterMark, m_DesignableGetter ? m_DesignableGetter->toJson() : QJsonValue(QString("")));
        result.insert(scriptableGetterMark, m_ScriptableGetter ? m_ScriptableGetter->toJson() : QJsonValue(QString("")));

        result.insert(revisionMark, m_Revision);

        result.insert(designableMark, m_Designable);
        result.insert(scriptableMark, m_Scriptable);
        result.insert(storedMark, m_Stored);
        result.insert(userMark, m_User);
        result.insert(constantMark, m_Constant);
        result.insert(finalMark, m_Final);

        return result;
    }

    /**
     * @brief Property::fromJson
     * @param src
     * @param errorList
     */
    void Property::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        using namespace utility;

        checkAndSet(src, nameMark, errorList, [&](){ m_Name = src[nameMark].toString(); });
        checkAndSet(src, idMark, errorList, [&](){ m_Id = src[idMark].toString(); });

        checkAndSet(src, fieldMark, errorList, [&](){ m_Field->fromJson( src[fieldMark].toObject(), errorList ); });

        checkAndSet(src, getterMark, errorList, [&](){ readOptionalMethod(src[getterMark], m_Getter, errorList); });
        checkAndSet(src, setterMark, errorList, [&](){ readOptionalMethod(src[setterMark], m_Setter, errorList); });
        checkAndSet(src, resetterMark, errorList, [&](){ readOptionalMethod(src[resetterMark], m_Resetter, errorList); });
        checkAndSet(src, notifierMark, errorList, [&](){ readOptionalMethod(src[notifierMark], m_Notifier, errorList); });

        checkAndSet(src, designableGetterMark, errorList, [&](){
            readOptionalMethod(src[designableGetterMark], m_DesignableGetter, errorList);
        });
        checkAndSet(src, scriptableGetterMark, errorList, [&](){
            readOptionalMethod(src[scriptableGetterMark], m_ScriptableGetter, errorList);
        });

        checkAndSet(src, revisionMark, errorList, [&](){ m_Revision = src[revisionMark].toInt(); });

        checkAndSet(src, designableMark, errorList, [&](){ m_Designable = src[designableMark].toBool(); });
        checkAndSet(src, scriptableMark, errorList, [&](){ m_Scriptable = src[scriptableMark].toBool(); });
        checkAndSet(src, storedMark, errorList, [&](){ m_Stored = src[storedMark].toBool(); });
        checkAndSet(src, userMark, errorList, [&](){ m_User = src[userMark].toBool(); });
        checkAndSet(src, constantMark, errorList, [&](){ m_Constant = src[constantMark].toBool(); });
        checkAndSet(src, finalMark, errorList, [&](){ m_Final = src[finalMark].toBool(); });
    }

    /**
     * @brief Property::hashType
     * @return
     */
    size_t Property::hashType() const
    {
        return Property::staticHashType();
    }

    /**
     * @brief Property::staticHashType
     * @return
     */
    size_t Property::staticHashType()
    {
        return typeid(Property).hash_code();
    }

    /**
     * @brief Property::marker
     * @return
     */
    QString Property::marker() const
    {
        return Property::staticMarker();
    }

    /**
     * @brief Property::staticMarker
     * @return
     */
    QString Property::staticMarker()
    {
        return ::marker;
    }

    /**
     * @brief Property::id
     * @return
     */
    QString Property::id() const
    {
        return m_Id;
    }

    /**
     * @brief Property::setId
     * @param id
     */
    void Property::setId(const QString &id)
    {
        m_Id = id;
    }

    /**
     * @brief Property::moveFrom
     * @param src
     */
    void Property::moveFrom(Property &&src)
    {
        m_Id = std::move(src.m_Id);

        m_Field = std::move(src.m_Field);

        m_Getter = std::move(src.m_Getter);
        m_Setter = std::move(src.m_Setter);
        m_Resetter = std::move(src.m_Resetter);
        m_Notifier = std::move(src.m_Notifier);

        m_DesignableGetter = std::move(src.m_DesignableGetter);
        m_ScriptableGetter = std::move(src.m_ScriptableGetter);

        m_Revision = std::move(src.m_Revision);

        m_Designable = std::move(src.m_Designable);
        m_Scriptable = std::move(src.m_Scriptable);
        m_Stored     = std::move(src.m_Stored);
        m_User       = std::move(src.m_User);
        m_Constant   = std::move(src.m_Constant);
        m_Final      = std::move(src.m_Final);
    }

    void Property::copyFrom(const Property &src)
    {
        m_Id = src.m_Id;

        m_Field = std::make_shared<Field>(*src.m_Field);

        m_Getter   = src.m_Getter ? std::make_shared<ClassMethod>(*src.m_Getter) : nullptr;
        m_Setter   = src.m_Setter ? std::make_shared<ClassMethod>(*src.m_Setter) : nullptr;
        m_Resetter = src.m_Resetter ? std::make_shared<ClassMethod>(*src.m_Resetter) : nullptr;
        m_Notifier = src.m_Notifier ? std::make_shared<ClassMethod>(*src.m_Notifier) : nullptr;

        m_DesignableGetter = src.m_DesignableGetter ? std::make_shared<ClassMethod>(*src.m_DesignableGetter) : nullptr;
        m_ScriptableGetter = src.m_ScriptableGetter ? std::make_shared<ClassMethod>(*src.m_ScriptableGetter) : nullptr;

        m_Revision = src.m_Revision;

        m_Designable = src.m_Designable;
        m_Scriptable = src.m_Scriptable;
        m_Stored     = src.m_Stored;
        m_User       = src.m_User;
        m_Constant   = src.m_Constant;
        m_Final      = src.m_Final;
    }

} // namespace entity

