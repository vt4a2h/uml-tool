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

#include <db/Database.h>
#include <entity/Type.h>
#include <entity/Class.h>
#include <entity/TemplateClass.h>
#include <entity/Union.h>
#include <entity/ClassMethod.h>
#include <entity/TemplateClassMethod.h>
#include <entity/Enum.h>
#include <entity/ExtendedType.h>
#include <relationship/association.h>
#include <relationship/dependency.h>
#include <relationship/generalization.h>
#include <relationship/multiplyassociation.h>
#include <relationship/realization.h>
#include <relationship/Relation.h>
#include <entity/Scope.h>

namespace utility {

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
        using KeywordsString = const std::map<entity::FieldKeyword,     QString>;
        using StringKeywords = const std::map<QString,     entity::FieldKeyword>;

        using LhsIdString    = const std::map<entity::LhsIdentificator, QString>;
        using StringLhsId    = const std::map<QString, entity::LhsIdentificator>;

        using RhsIdString    = const std::map<entity::RhsIdentificator, QString>;
        using StringRhsId    = const std::map<QString, entity::RhsIdentificator>;

        using SectionString  = const std::map<entity::Section,          QString>;
        using UserTypeString = const std::map<entity::UserType,         QString>;

        KeywordsString kKeywords { {entity::Volatile, "volatile"}, {entity::Mutable, "mutable"},
                                   {entity::FieldStatic, "static"}
                                 };
        StringKeywords kStrKey { {"volatile", entity::Volatile}, {"mutable", entity::Mutable},
                                 {"static", entity::FieldStatic}
                               };

        LhsIdString    kLhsId    { {entity::LhsIdentificator::Explicit, "explicit"},
                                   {entity::LhsIdentificator::Inline, "inline"},
                                   {entity::LhsIdentificator::MethodStatic, "static"},
                                   {entity::LhsIdentificator::Virtual, "virtual"},
                                   {entity::LhsIdentificator::Friend, "friend"},
                                   {entity::LhsIdentificator::None, ""}
                                 };

        StringLhsId   kSLhsId    { {"explicit", entity::LhsIdentificator::Explicit},
                                   {"inline", entity::LhsIdentificator::Inline},
                                   {"static", entity::LhsIdentificator::MethodStatic},
                                   {"virtual", entity::LhsIdentificator::Virtual},
                                   {"friend", entity::LhsIdentificator::Friend},
                                   {"", entity::LhsIdentificator::None}
                                 };

        RhsIdString    kRhsId    { {entity::RhsIdentificator::None, ""},
                                   {entity::RhsIdentificator::Override, "override"},
                                   {entity::RhsIdentificator::Final, "final"},
                                   {entity::RhsIdentificator::Delete, "= delete"},
                                   {entity::RhsIdentificator::Default, "= default"},
                                   {entity::RhsIdentificator::PureVirtual, "= 0"}
                                 };

        StringRhsId    kSRhsId   { {"", entity::RhsIdentificator::None},
                                   {"override", entity::RhsIdentificator::Override},
                                   {"final", entity::RhsIdentificator::Final},
                                   {"delete", entity::RhsIdentificator::Delete},
                                   {"default", entity::RhsIdentificator::Default},
                                   {"0", entity::RhsIdentificator::PureVirtual}
                                 };

        SectionString  kSection  { {entity::Public, "public"}, {entity::Protected, "protected"},
                                   {entity::Private, "private"}, {entity::None, ""}
                                 };

        SectionString  kSectionSymbol { {entity::Public, "+"}, {entity::Protected, "#"},
                                        {entity::Private, "-"}, {entity::None, ""}
                                      };

        using MakerT  = std::function<entity::SharedType()>;
        using MakersT = const std::map<QString, MakerT>;
        using MakerM  = std::function<entity::SharedMethod()>;
        using MakersM = const std::map<entity::ClassMethodType, MakerM>;

        MakersT kType =
            { {entity::Type::staticMarker(),          [](){ return std::make_shared<entity::Type>();          }},
              {entity::Class::staticMarker(),         [](){ return std::make_shared<entity::Class>();         }},
              {entity::TemplateClass::staticMarker(), [](){ return std::make_shared<entity::TemplateClass>(); }},
              {entity::Union::staticMarker(),         [](){ return std::make_shared<entity::Union>();         }},
              {entity::Enum::staticMarker(),          [](){ return std::make_shared<entity::Enum>();          }},
              {entity::ExtendedType::staticMarker(),  [](){ return std::make_shared<entity::ExtendedType>();  }}
            };

        MakersM kMethod = { {entity::SimpleMethod,   [](){ return std::make_shared<entity::ClassMethod>();         }},
                            {entity::TemplateMethod, [](){ return std::make_shared<entity::TemplateClassMethod>(); }}
                          };
    }

    /**
     * @brief makeType
     * @param type
     * @return
     */
    std::shared_ptr<entity::Type> makeType(const QString &hash)
    {
        MakerT defaultMaker([](){ return std::make_shared<entity::Type>(); });

        return mapSearchHelper(kType, hash, defaultMaker)();
    }

    /**
     * @brief makeMethod
     * @param methodType
     * @return
     */
    std::shared_ptr<entity::ClassMethod> makeMethod(entity::ClassMethodType methodType)
    {
        MakerM defaultMaker([](){ return std::make_shared<entity::ClassMethod>(); });

        return mapSearchHelper(kMethod, methodType, defaultMaker)();
    }

    /**
     * @brief fieldKeywordToString
     * @param keyword
     * @return
     */
    QString fieldKeywordToString(entity::FieldKeyword keyword)
    {
        return mapSearchHelper(kKeywords, keyword, QString("unknown"));
    }

    /**
     * @brief methodLhsIdToString
     * @param id
     * @return
     */
    QString methodLhsIdToString(entity::LhsIdentificator id)
    {
        return mapSearchHelper(kLhsId, id, QString("unknown"));
    }

    /**
     * @brief methodRhsIdToString
     * @param id
     * @return
     */
    QString methodRhsIdToString(entity::RhsIdentificator id)
    {
        return mapSearchHelper(kRhsId, id, QString("unknown"));
    }

    /**
     * @brief sectionToString
     * @param section
     * @return
     */
    QString sectionToString(entity::Section section)
    {
        return mapSearchHelper(kSection, section, QString("unknown"));
    }

    /**
     * @brief scopesNamesList
     * @param type
     * @param db
     * @return
     */
    QStringList scopesNamesList(const entity::SharedType &type, const db::SharedDatabase &db)
    {
        QStringList result;
        entity::SharedScope scope(db->scope(type->scopeId(), true /*searchInDepth*/));
        auto id(scope ? scope->id() : common::ID::globalScopeID());

        while (scope && id != common::ID::globalScopeID()) {
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
    entity::FieldKeyword fieldKeywordFromString(const QString &keyword)
    {
        return mapSearchHelper(kStrKey, keyword, entity::FieldKeyword::Invalid);
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
    entity::LhsIdentificator methodLhsIdFromString(const QString &in)
    {
        return mapSearchHelper(kSLhsId, in, entity::LhsIdentificator::None);
    }

    /**
     * @brief methodRhsIdFromString
     * @param in
     * @return
     */
    entity::RhsIdentificator methodRhsIdFromString(const QString &in)
    {
        return mapSearchHelper(kSRhsId, in, entity::RhsIdentificator::None);
    }

    /**
     * @brief sectionToSymbol
     * @param section
     * @return
     */
    QString sectionToSymbol(entity::Section section)
    {
        return mapSearchHelper(kSectionSymbol, section, QString("unknown"));
    }

}
