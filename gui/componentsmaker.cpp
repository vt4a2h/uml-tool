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

namespace gui {

    namespace {
        using Keywords = QSet<QString>;
        using CapIndexKeywords = std::pair<int, Keywords>; // Capture index, keywords which MUST NOT contains in captured text

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

        // NOTE: Just simple patterns now, must be improved in future (prefer to use simple parser)
        const QString fieldPattern = "^((?:volatile|static|mutable)\\s)?" // 1 -- lhs keywords
                                     "(const\\s)?"                        // 2 -- const
                                     "((?:\\w+:{2,})*)"                   // 3 -- namespaces
                                     "(\\w+)"                             // 4 -- typename
                                     "\\s+([\\*\\s\\&const]*)"            // 5 -- &*const
                                     "\\s*(\\w+)$";                       // 6 -- field name

        const QMap<models::DisplayPart, QString> componentPatternMap =
        {
            {models::DisplayPart::Fields, fieldPattern},
        };

        const QMap<models::DisplayPart, QVector<CapIndexKeywords>> componentIndexesMap =
        {
            {models::DisplayPart::Fields, {{3, Keywords()}, {4, reservedKeywords}, {6, types}}},
        };

        bool notContainsInvalidKeyword(const QRegularExpressionMatch &match, models::DisplayPart display, QStringList &captured)
        {
            for (auto &&indexKeywords : componentIndexesMap[display]) {
                const QString &cap = match.captured(indexKeywords.first).trimmed();
                const QStringList &tmpList = cap.split("::", QString::SkipEmptyParts);
                if (!(tmpList.toSet() & indexKeywords.second).isEmpty()) {
                    captured.clear();
                    return false;
                }

                captured.append(tmpList);
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
    entity::SharedBasicEntity ComponentsMaker::makeComponent(const QString &signature, models::DisplayPart display)
    {
        Q_UNUSED(signature);
        Q_UNUSED(display);

        return entity::SharedBasicEntity();
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

} // namespace gui

