/*****************************************************************************
**
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 29/10/2014.
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

#include "helpfunctions.h"

#include "enums.h"
#include "Constants.h"

#include <functional>

#include <QUuid>
#include <QString>
#include <QJsonObject>
#include <QStringList>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include <DB/Database.h>
#include <Entity/Type.h>
#include <Entity/Class.h>
#include <Entity/TemplateClass.h>
#include <Entity/Union.h>
#include <Entity/ClassMethod.h>
#include <Entity/TemplateClassMethod.h>
#include <Entity/Enum.h>
#include <Entity/ExtendedType.h>
#include <Relationship/association.h>
#include <Relationship/dependency.h>
#include <Relationship/generalization.h>
#include <Relationship/multiplyassociation.h>
#include <Relationship/realization.h>
#include <Relationship/Relation.h>
#include <Entity/Scope.h>

namespace Util {

    /**
     * @brief genId
     * @return
     */
    QString genId()
    {
        return QUuid::createUuid().toString();
    }

    /**
     * @brief checkAndSet
     * @param object
     * @param key
     * @param lst
     * @param func
     */
    void checkAndSet(const QJsonObject &object, const QString &key, QStringList &lst, const std::function<void()> &func)
    {
        object.contains(key) ? func() : lst.append(QObject::tr("Key \"%1\" not found!").arg(key));
    }

    namespace {
        using KeywordsString = const std::map<Entity::FieldKeyword,     QString>;
        using StringKeywords = const std::map<QString,     Entity::FieldKeyword>;

        using LhsIdString    = const std::map<Entity::LhsIdentificator, QString>;
        using StringLhsId    = const std::map<QString, Entity::LhsIdentificator>;

        using RhsIdString    = const std::map<Entity::RhsIdentificator, QString>;
        using StringRhsId    = const std::map<QString, Entity::RhsIdentificator>;

        using SectionString  = const std::map<Entity::Section,          QString>;
        using UserTypeString = const std::map<Entity::UserType,         QString>;

        KeywordsString kKeywords { {Entity::Volatile, "volatile"}, {Entity::Mutable, "mutable"},
                                   {Entity::FieldStatic, "static"}
                                 };
        StringKeywords kStrKey { {"volatile", Entity::Volatile}, {"mutable", Entity::Mutable},
                                 {"static", Entity::FieldStatic}
                               };

        LhsIdString    kLhsId    { {Entity::LhsIdentificator::Explicit, "explicit"},
                                   {Entity::LhsIdentificator::Inline, "inline"},
                                   {Entity::LhsIdentificator::MethodStatic, "static"},
                                   {Entity::LhsIdentificator::Virtual, "virtual"},
                                   {Entity::LhsIdentificator::Friend, "friend"},
                                   {Entity::LhsIdentificator::None, ""}
                                 };

        StringLhsId   kSLhsId    { {"explicit", Entity::LhsIdentificator::Explicit},
                                   {"inline", Entity::LhsIdentificator::Inline},
                                   {"static", Entity::LhsIdentificator::MethodStatic},
                                   {"virtual", Entity::LhsIdentificator::Virtual},
                                   {"friend", Entity::LhsIdentificator::Friend},
                                   {"", Entity::LhsIdentificator::None}
                                 };

        RhsIdString    kRhsId    { {Entity::RhsIdentificator::None, ""},
                                   {Entity::RhsIdentificator::Override, "override"},
                                   {Entity::RhsIdentificator::Final, "final"},
                                   {Entity::RhsIdentificator::Delete, "= delete"},
                                   {Entity::RhsIdentificator::Default, "= default"},
                                   {Entity::RhsIdentificator::PureVirtual, "= 0"}
                                 };

        StringRhsId    kSRhsId   { {"", Entity::RhsIdentificator::None},
                                   {"override", Entity::RhsIdentificator::Override},
                                   {"final", Entity::RhsIdentificator::Final},
                                   {"delete", Entity::RhsIdentificator::Delete},
                                   {"default", Entity::RhsIdentificator::Default},
                                   {"0", Entity::RhsIdentificator::PureVirtual}
                                 };

        SectionString  kSection  { {Entity::Public, "public"}, {Entity::Protected, "protected"},
                                   {Entity::Private, "private"}, {Entity::None, ""}
                                 };

        SectionString  kSectionSymbol { {Entity::Public, "+"}, {Entity::Protected, "#"},
                                        {Entity::Private, "-"}, {Entity::None, ""}
                                      };

        using MakerT  = std::function<Entity::SharedType()>;
        using MakersT = const std::map<QString, MakerT>;
        using MakerM  = std::function<Entity::SharedMethod()>;
        using MakersM = const std::map<Entity::ClassMethodType, MakerM>;

        MakersT kType =
            { {Entity::Type::staticMarker(),          [](){ return std::make_shared<Entity::Type>();          }},
              {Entity::Class::staticMarker(),         [](){ return std::make_shared<Entity::Class>();         }},
              {Entity::TemplateClass::staticMarker(), [](){ return std::make_shared<Entity::TemplateClass>(); }},
              {Entity::Union::staticMarker(),         [](){ return std::make_shared<Entity::Union>();         }},
              {Entity::Enum::staticMarker(),          [](){ return std::make_shared<Entity::Enum>();          }},
              {Entity::ExtendedType::staticMarker(),  [](){ return std::make_shared<Entity::ExtendedType>();  }}
            };

        MakersM kMethod = { {Entity::SimpleMethod,   [](){ return std::make_shared<Entity::ClassMethod>();         }},
                            {Entity::TemplateMethod, [](){ return std::make_shared<Entity::TemplateClassMethod>(); }}
                          };
    }

    /**
     * @brief makeType
     * @param type
     * @return
     */
    std::shared_ptr<Entity::Type> makeType(const QString &hash)
    {
        MakerT defaultMaker([](){ return std::make_shared<Entity::Type>(); });

        return mapSearchHelper(kType, hash, defaultMaker)();
    }

    /**
     * @brief makeMethod
     * @param methodType
     * @return
     */
    std::shared_ptr<Entity::ClassMethod> makeMethod(Entity::ClassMethodType methodType)
    {
        MakerM defaultMaker([](){ return std::make_shared<Entity::ClassMethod>(); });

        return mapSearchHelper(kMethod, methodType, defaultMaker)();
    }

    /**
     * @brief fieldKeywordToString
     * @param keyword
     * @return
     */
    QString fieldKeywordToString(Entity::FieldKeyword keyword)
    {
        return mapSearchHelper(kKeywords, keyword, QString("unknown"));
    }

    /**
     * @brief methodLhsIdToString
     * @param id
     * @return
     */
    QString methodLhsIdToString(Entity::LhsIdentificator id)
    {
        return mapSearchHelper(kLhsId, id, QString("unknown"));
    }

    /**
     * @brief methodRhsIdToString
     * @param id
     * @return
     */
    QString methodRhsIdToString(Entity::RhsIdentificator id)
    {
        return mapSearchHelper(kRhsId, id, QString("unknown"));
    }

    /**
     * @brief sectionToString
     * @param section
     * @return
     */
    QString sectionToString(Entity::Section section)
    {
        return mapSearchHelper(kSection, section, QString("unknown"));
    }

    /**
     * @brief scopesNamesList
     * @param type
     * @param db
     * @return
     */
    QStringList scopesNamesList(const Entity::SharedType &type, const DB::SharedDatabase &db)
    {
        QStringList result;
        Entity::SharedScope scope(db->scope(type->scopeId(), true /*searchInDepth*/));
        auto id(scope ? scope->id() : Common::ID::globalScopeID());

        while (scope && id != Common::ID::globalScopeID()) {
            result << scope->name();
            id = scope->scopeId();
            scope = db->scope(id, true /*searchInDepth*/);
        }

        return result;
    }

    /**
     * @brief fieldKeywordFromString
     * @param keyword
     * @return
     */
    Entity::FieldKeyword fieldKeywordFromString(const QString &keyword)
    {
        return mapSearchHelper(kStrKey, keyword, Entity::FieldKeyword::Invalid);
    }

    /**
     * @brief toBool
     * @param in
     * @param ok
     * @return
     */
    bool toBool(const QString &in, bool &ok)
    {
        auto match = QRegularExpression("^(true|false)$").match(in);
        return (ok = match.hasMatch()) ? match.captured(1) == "true" ? true : false : ok;
    }

    /**
     * @brief methodLhsIdFromString
     * @param in
     * @return
     */
    Entity::LhsIdentificator methodLhsIdFromString(const QString &in)
    {
        return mapSearchHelper(kSLhsId, in, Entity::LhsIdentificator::None);
    }

    /**
     * @brief methodRhsIdFromString
     * @param in
     * @return
     */
    Entity::RhsIdentificator methodRhsIdFromString(const QString &in)
    {
        return mapSearchHelper(kSRhsId, in, Entity::RhsIdentificator::None);
    }

    /**
     * @brief sectionToSymbol
     * @param section
     * @return
     */
    QString sectionToSymbol(Entity::Section section)
    {
        return mapSearchHelper(kSectionSymbol, section, QString("unknown"));
    }

}
