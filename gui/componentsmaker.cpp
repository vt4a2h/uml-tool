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

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>

#include <entity/field.h>
#include <entity/scope.h>
#include <entity/type.h>
#include <entity/extendedtype.h>

#include <models/applicationmodel.h>

#include <utility/helpfunctions.h>

#include "enums.h"

namespace gui {

    namespace {
        using Keywords = QSet<QString>;

        const Keywords types = {"bool", "char16_t", "char32_t", "float", "int", "long", "short", "signed",
                                "wchar_t", "double", "void" };
        const Keywords reservedKeywords = { "alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand", "bitor",
                                            "break", "case", "catch", "char",  "class",
                                            "compl", "const", "constexpr", "const_cast", "continue", "decltype",
                                            "default", "delete", "do", "dynamic_cast", "else", "enum",
                                            "explicit", "export", "extern", "false", "for", "friend", "goto",
                                            "if", "inline",  "mutable", "namespace", "new", "noexcept",
                                            "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private",
                                            "protected", "public", "register", "reinterpret_cast", "return",
                                            "sizeof", "static", "static_assert", "static_cast", "struct",
                                            "switch", "template", "this", "thread_local", "throw", "true", "try",
                                            "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual",
                                            "volatile", "while", "xor", "xor_eq" };

        enum class FieldGroupNames { LhsKeywords = 1, ConstStatus, Namespaces, Typename, PLC, Name, GroupsCount };

        // TODO: Just simple patterns now, must be improved in future (prefer to use simple parser)
        // TODO: 5 section may contains wrong combination of "*&const" it must be fixed.
        const QString fieldPattern = "^((?:volatile|static|mutable)\\s)?" // 1 -- lhs keywords
                                     "(const\\s)?"                        // 2 -- const
                                     "((?:\\w*:{2,})*)"                   // 3 -- namespaces
                                     "(\\w+)"                             // 4 -- typename
                                     "\\s+([\\*\\s\\&const]*)"            // 5 -- &*const
                                     "\\s*(\\w+)$";                       // 6 -- field name

        const QMap<models::DisplayPart, QString> componentPatternMap =
        {
            {models::DisplayPart::Fields, fieldPattern},
        };

        const QMap<models::DisplayPart, int> componentsGroupCount =
        {
            {models::DisplayPart::Fields, int(FieldGroupNames::GroupsCount)},
        };

        // Capture index, keywords which MUST NOT contains in captured text
        using CapIndexKeywords = std::pair<FieldGroupNames, Keywords>;

        const QMap<models::DisplayPart, QVector<CapIndexKeywords>> componentIndexesMap =
        {
            {models::DisplayPart::Fields, {{FieldGroupNames::Namespaces, reservedKeywords|types},
                                           {FieldGroupNames::Typename, reservedKeywords},
                                           {FieldGroupNames::Name, types}}},
        };

        using MakerFunction = std::function<MessageEntity()>;
        QMap<models::DisplayPart, MakerFunction> componentMakerMap;

        bool notContainsInvalidKeyword(const QRegularExpressionMatch &match, models::DisplayPart display,
                                       QVector<QString> &captured)
        {
            const int groupsCount = int(componentsGroupCount[display]);
            captured.resize(groupsCount);

            const QVector<CapIndexKeywords> &rules = componentIndexesMap[display];
            for (int groupIndex = 1; groupIndex < groupsCount; ++groupIndex)
            {
                const QString &cap = match.captured(groupIndex).trimmed();
                captured[groupIndex] = cap;

                auto it = utility::find_if(rules, [&](const CapIndexKeywords &c){ return int(c.first) == groupIndex; });
                if (it != cend(rules)) {
                    const QStringList &tmpList = cap.split("::", QString::SkipEmptyParts);
                    if (!(tmpList.toSet() & it->second).isEmpty()) {
                        captured.clear();
                        return false;
                    }
                }
            }

            return true;
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
        , m_LastSignature("")
    {
        componentMakerMap[models::DisplayPart::Fields] = [&]{ return makeField(); };
    }

    /**
     * @brief ComponentsMaker::signatureValid
     * @param signature
     * @return
     */
    bool ComponentsMaker::signatureValid(const QString &signature, models::DisplayPart display)
    {
        const QString &pattern = componentPatternMap[display];
        if (pattern.isEmpty())
            return false;

        const QRegularExpression re(pattern);
        const auto match =  re.match(signature.trimmed());
        if (match.hasMatch()) {
            m_LastCaptured.clear();
            m_LastSignature.clear();

            const bool result = notContainsInvalidKeyword(match, display, m_LastCaptured);
            if (result) {
                m_LastSignature = signature;
                return result;
            }
        }

        return false;
    }

    /**
     * @brief ComponentsMaker::makeComponent
     * @param signature
     * @return
     */
    MessageEntity ComponentsMaker::makeComponent(const QString &signature, models::DisplayPart display)
    {
        if ((signature.isEmpty() && !m_LastSignature.isEmpty() && !m_LastCaptured.isEmpty()) ||
            (signature == m_LastSignature && !m_LastCaptured.isEmpty()) ||
            signatureValid(signature, display))
            return componentMakerMap[display]();

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
    MessageEntity ComponentsMaker::makeField()
    {
        Q_ASSERT(!m_LastCaptured.isEmpty() && !m_LastCaptured[int(FieldGroupNames::Typename)].isEmpty() &&
                 !m_LastCaptured[int(FieldGroupNames::Name)].isEmpty());

        Q_ASSERT(m_Model);
        Q_ASSERT(m_Model->globalDatabase());
        Q_ASSERT(m_Model->currentProject());
        Q_ASSERT(m_Model->currentProject()->database());

        auto newField = std::make_shared<entity::Field>();
        newField->setName(m_LastCaptured[int(FieldGroupNames::Name)]);
        if (!m_LastCaptured[int(FieldGroupNames::LhsKeywords)].isEmpty()) {
            auto keyword = utility::fieldKeywordFromString(m_LastCaptured[int(FieldGroupNames::LhsKeywords)]);
            Q_ASSERT(keyword != entity::FieldKeyword::Invalid);
            newField->addKeyword(keyword);
        }
        qDebug() << "created new field with name: " << m_LastCaptured[int(FieldGroupNames::Name)];

        const QString &typeName = m_LastCaptured[int(FieldGroupNames::Typename)];
        entity::SharedType type;

        if (!m_LastCaptured[int(FieldGroupNames::Namespaces)].isEmpty()) {
            auto names = m_LastCaptured[int(FieldGroupNames::Namespaces)].split("::");
            auto scope = m_Model->globalDatabase()->chainScopeSearch(names);
            if (!scope)
                scope = m_Model->currentProject()->database()->chainScopeSearch(names);

            if (scope)
                type = scope->typeByName(typeName);
        } else {
            // First of all check in all scopes of global database
            const entity::ScopesList &scopes = m_Model->globalDatabase()->scopes();
            utility::find_if(scopes, [&](auto &&scope){ type = scope->typeByName(typeName); return !!type; });

            // If not found, try to check project database
            if (!type) {
                auto db = m_Model->currentProject()->database();
                utility::find_if(db->scopes(), [&](auto &&scope){ type = scope->typeByName(typeName); return !!type; });
            }
        }
        qDebug() << "found type: " << !!type << ", with name: " << typeName;
        if (!type)
            return {tr("Wrong type: %1.").arg(typeName), nullptr};

        entity::SharedExtendedType extendedType = std::make_shared<entity::ExtendedType>();
        extendedType->setTypeId(type->id());
        extendedType->setScopeId(m_Scope->id());
        extendedType->setConstStatus(!m_LastCaptured[int(FieldGroupNames::ConstStatus)].isEmpty());

        if (!m_LastCaptured[int(FieldGroupNames::PLC)].isEmpty()) {
            QString plc = m_LastCaptured[int(FieldGroupNames::PLC)];
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

        if (extendedType->isConst() || !extendedType->pl().isEmpty()) {
            QStringList namespaces = m_LastCaptured[int(FieldGroupNames::Namespaces)]
                                    .remove(QChar::Space)
                                    .split("::", QString::SkipEmptyParts);
            if (!namespaces.isEmpty()) {
            } else {
                const entity::TypesList &types = m_Scope->types();
                auto it = utility::find_if(types, [&](const entity::SharedType &type) {
                                                      return extendedType->isEqual(*type, false);
                                                  });
                if (it == cend(types))
                    m_Scope->addExistsType(extendedType);
            }

            newField->setTypeId(extendedType->id());
         } else {
            newField->setTypeId(type->id());
         }

        return {"", newField};
    }

} // namespace gui

