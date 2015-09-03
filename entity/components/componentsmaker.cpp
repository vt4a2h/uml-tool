/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 12/07/2015.
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
#include "componentsmaker.h"

#include <entity/field.h>
#include <entity/scope.h>
#include <entity/type.h>
#include <entity/extendedtype.h>
#include <entity/property.h>

#include <models/applicationmodel.h>

#include <utility/helpfunctions.h>

#include "enums.h"
#include "componentscommon.h"
#include "constants.h"

namespace components {

    namespace {

        template <class Method>
        inline void addCommon(const Tokens &tokens, PropGroupNames group, Method method, entity::SharedProperty &property)
        {
            const auto &name = tokens[int(group)];
            if (!name.isEmpty())
                (property.get()->*method)(name);
        }

        template <class AddMethod, class SetMethod>
        inline void addDS(const Tokens &tokens, PropGroupNames group, AddMethod addMethod, SetMethod setMethod,
                          entity::SharedProperty &property)
        {
            const QString &name = tokens[int(group)];
            bool ok = false;
            bool result = utility::toBool(name, ok);

            if (ok)
                (property.get()->*setMethod)(result);
            else
                (property.get()->*addMethod)(name);
        }
    }

    /**
     * @brief ComponentsMaker::ComponentsMaker
     */
    ComponentsMaker::ComponentsMaker()
        : ComponentsMaker(nullptr, nullptr, nullptr)
    {
    }

    /**
     * @brief ComponentsMaker::ComponentsMaker
     * @param model
     * @param entity
     * @param scope
     */
    ComponentsMaker::ComponentsMaker(const models::SharedApplicationModel &model, const entity::SharedType &entity,
                                     const entity::SharedScope &scope)
        : m_Model(model)
        , m_Entity(entity)
        , m_Scope(scope)
    {
        m_ComponentMakerMap[models::DisplayPart::Fields]     = [&](auto tokens){ return this->makeField(tokens);    };
        m_ComponentMakerMap[models::DisplayPart::Properties] = [&](auto tokens){ return this->makeProperty(tokens); };
    }

    /**
     * @brief ComponentsMaker::makeComponent
     * @param signature
     * @return
     */
    OptionalEntity ComponentsMaker::makeComponent(const components::Tokens &tokens, models::DisplayPart display)
    {
        if (!tokens.isEmpty())
            return m_ComponentMakerMap[display](tokens);

        return {tr("Wrong signature"), entity::SharedBasicEntity()};
    }

    /**
     * @brief ComponentsMaker::model
     * @return
     */
    models::SharedApplicationModel ComponentsMaker::model() const
    {
        return m_Model;
    }

    /**
     * @brief ComponentsMaker::setModel
     * @param model
     */
    void ComponentsMaker::setModel(const models::SharedApplicationModel &model)
    {
        m_Model = model;
    }

    /**
     * @brief ComponentsMaker::entity
     * @return
     */
    entity::SharedType ComponentsMaker::entity() const
    {
        return m_Entity;
    }

    /**
     * @brief ComponentsMaker::setEntity
     * @param entity
     */
    void ComponentsMaker::setEntity(const entity::SharedType &entity)
    {
        m_Entity = entity;
    }

    /**
     * @brief ComponentsMaker::scope
     * @return
     */
    entity::SharedScope ComponentsMaker::scope() const
    {
        return m_Scope;
    }

    /**
     * @brief ComponentsMaker::setScope
     * @param scope
     */
    void ComponentsMaker::setScope(const entity::SharedScope &scope)
    {
        m_Scope = scope;
    }

    /**
     * @brief ComponentsMaker::makeField
     * @return
     */
    OptionalEntity ComponentsMaker::makeField(const Tokens &tokens)
    {
        Q_ASSERT(!tokens.isEmpty() && !tokens[int(FieldGroupNames::Typename)].isEmpty() &&
                 !tokens[int(FieldGroupNames::Name)].isEmpty());

        Q_ASSERT(m_Model);
        Q_ASSERT(m_Model->globalDatabase());
        Q_ASSERT(m_Model->currentProject());
        Q_ASSERT(m_Model->currentProject()->database());

        auto tmpTokens = tokens;

        auto newField = std::make_shared<entity::Field>();
        newField->setName(tmpTokens[int(FieldGroupNames::Name)]);
        if (!tmpTokens[int(FieldGroupNames::LhsKeywords)].isEmpty()) {
            auto keyword = utility::fieldKeywordFromString(tmpTokens[int(FieldGroupNames::LhsKeywords)]);
            Q_ASSERT(keyword != entity::FieldKeyword::Invalid);
            newField->addKeyword(keyword);
        }

        const QString &typeName = tmpTokens[int(FieldGroupNames::Typename)];
        entity::SharedType type;

        if (!tmpTokens[int(FieldGroupNames::Namespaces)].isEmpty()) {
            auto names = tmpTokens[int(FieldGroupNames::Namespaces)].split("::", QString::SkipEmptyParts);
            auto scope = m_Model->globalDatabase()->chainScopeSearch(names);
            if (!scope)
                scope = m_Model->currentProject()->database()->chainScopeSearch(names);

            if (scope)
                type = scope->typeByName(typeName);
        } else {
            // First of all check in all scopes of global database
            const entity::ScopesList &scopes = m_Model->globalDatabase()->scopes();
            utility::find_if(scopes, [&](auto scope){ type = scope->typeByName(typeName); return !!type; });

            // If not found, try to check project database
            if (!type) {
                auto db = m_Model->currentProject()->database();
                utility::find_if(db->scopes(), [&](auto scope){ type = scope->typeByName(typeName); return !!type; });
            }
        }

        if (!type)
            return {tr("Wrong type: %1.").arg(typeName), nullptr};

        entity::SharedExtendedType extendedType = std::make_shared<entity::ExtendedType>();
        extendedType->setTypeId(type->id());
        extendedType->setScopeId(m_Scope->id());
        extendedType->setConstStatus(!tmpTokens[int(FieldGroupNames::ConstStatus)].isEmpty());

        if (!tmpTokens[int(FieldGroupNames::PLC)].isEmpty()) {
            QString plc = tmpTokens[int(FieldGroupNames::PLC)];
            plc.remove(QChar::Space);

            if (plc.startsWith("const")) {
                extendedType->setConstStatus(true);
                plc.remove(0, 4);
            }

            while (!plc.isEmpty()) {
                if (plc.startsWith("const")) {
                    plc.remove(0, 5);
                } else if (plc.startsWith("*const")) {
                    extendedType->addPointerStatus(true);
                    plc.remove(0, 6);
                } else if (plc.startsWith("*")) {
                    extendedType->addPointerStatus();
                    plc.remove(0, 1);
                } else if (plc.startsWith("&")) {
                    extendedType->addLinkStatus();
                    plc.remove(0, 1);
                }
            }
        }

        if (!tmpTokens[int(FieldGroupNames::TemplateArgs)].isEmpty()) {
            QStringList arguments = tmpTokens[int(FieldGroupNames::TemplateArgs)]
                                    .remove(QChar::Space)
                                    .split(",", QString::SkipEmptyParts);
            entity::ScopesList scopes = m_Model->currentProject()->database()->scopes();
            scopes.append(m_Model->globalDatabase()->scopes());

            // TODO: add namespaces, * and const
            for (auto &&name : arguments) {
                entity::SharedType t;
                utility::find_if(scopes, [&](auto &&sc){ t = sc->typeByName(name); return !!t; });
                if (t)
                    extendedType->addTemplateParameter(t->id());
                else
                    return {tr("Template parameter \"%1\" not found.").arg(name), nullptr};
            }
        }

        if (extendedType->isConst() || !extendedType->templateParameters().isEmpty() || !extendedType->pl().isEmpty()) {
            const entity::TypesList &types = m_Scope->types();
            auto it = utility::find_if(types, [=](const entity::SharedType &type) {
                                                  return extendedType->isEqual(*type, false);
                                              });
            if (it == cend(types))
                m_Model->addExistsType(m_Model->currentProject()->id(), m_Scope->id(), extendedType);

            newField->setTypeId(extendedType->id());
         } else {
            newField->setTypeId(type->id());
         }

        return {"", newField};
    }

    OptionalEntity ComponentsMaker::makeProperty(const Tokens &tokens)
    {
        Q_ASSERT(!tokens.isEmpty() && !tokens[int(PropGroupNames::Type)].isEmpty() &&
                 !tokens[int(PropGroupNames::Name)].isEmpty());

        Q_ASSERT(m_Model);
        Q_ASSERT(m_Model->globalDatabase());
        Q_ASSERT(m_Model->currentProject());
        Q_ASSERT(m_Model->currentProject()->database());

        entity::SharedProperty newProperty = std::make_shared<entity::Property>();

        // Add name
        newProperty->setName(tokens[int(PropGroupNames::Name)]);

        // Add type
        // Not support namespaces for now, so check only in global database.
        // Work with namespaces and custom types in project will be hard due to Qt meta-stuff.
        // And also required more detail work on current code generation functionality. TODO: implement!
        const entity::SharedScope &globasScope = m_Model->globalDatabase()->getScope(GLOBAL_SCOPE_ID);
        if (!globasScope)
            return {tr("Cannot find global scope."), nullptr};

        const auto &typeName = tokens[int(PropGroupNames::Type)];
        entity::SharedType type = globasScope->typeByName(typeName);
        if (!type)
            return {tr("Wrong type: %1.").arg(typeName), nullptr};

        // Member (supports only "m_" prefix for now)
        const auto &member = tokens[int(PropGroupNames::Member)];
        if (!member.isEmpty()) {
            const bool hasPrefix = member.startsWith("m_");
            const auto name(hasPrefix ? QString(member).remove(0, 2) : member);
            const auto prefix(hasPrefix ? "m_" : "");
            newProperty->addMember(name, prefix);
        }

        // Common methods
        addCommon(tokens, PropGroupNames::Getter,   &entity::Property::addGetter,   newProperty);
        addCommon(tokens, PropGroupNames::Setter,   &entity::Property::addSetter,   newProperty);
        addCommon(tokens, PropGroupNames::Resetter, &entity::Property::addResetter, newProperty);
        addCommon(tokens, PropGroupNames::Notifier, &entity::Property::addNotifier, newProperty);

        // Revision
        const auto &revision = tokens[int(PropGroupNames::Revision)];
        if (!revision.isEmpty()) {
            bool ok = false;
            int rev = revision.toInt(&ok);

            if (ok)
                newProperty->setRevision(rev);
            else
                return {tr("Wrong revision: %1.").arg(revision), nullptr};
        }

        // Add designable and scriptable
        addDS(tokens, PropGroupNames::Designable, &entity::Property::addDesignableGetter,
              &entity::Property::setDesignable, newProperty);
        addDS(tokens, PropGroupNames::Scriptable, &entity::Property::addScriptableGetter,
              &entity::Property::setScriptable, newProperty);

        return {"", newProperty};
    }

} // namespace components

