/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 29/08/2015.
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
#include "componentsignatureparser.h"

#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include <models/componentsmodel.h>

#include <utility/helpfunctions.h>

#include "token.h"
#include "componentscommon.h"

namespace components {

    namespace {
        using Keywords = QSet<QString>;

        const Keywords types = {"bool", "char16_t", "char32_t", "float", "int", "long", "short",
                                "signed", "wchar_t", "double", "void" };
        const Keywords reservedKeywords = { "alignas", "alignof", "and", "and_eq", "asm", "auto",
                                            "bitand", "bitor", "break", "case", "catch", "char",
                                            "class", "compl", "const", "constexpr", "const_cast",
                                            "continue", "decltype", "default", "delete", "do",
                                            "dynamic_cast", "else", "enum", "explicit", "export",
                                            "extern", "for", "friend", "goto", "if", "inline",
                                            "mutable", "namespace", "new", "noexcept", "not",
                                            "not_eq", "nullptr", "operator", "or", "or_eq",
                                            "private", "protected", "public", "register",
                                            "reinterpret_cast", "return", "sizeof", "static",
                                            "static_assert", "static_cast", "struct", "switch",
                                            "template", "this", "thread_local", "throw", "try",
                                            "typedef", "typeid", "typename", "union", "unsigned",
                                            "using", "virtual", "volatile", "while", "xor", "xor_eq"
                                          };
        const Keywords boolKeywords = { "true", "false" };
        const Keywords propKeywords = { "MEMBER", "READ", "WRITE", "RESET", "NOTIFY", "REVISION",
                                        "DESIGNABLE", "SCRIPTABLE", "STORED", "USER", "CONSTANT",
                                        "FINAL" };

        // TODO: Just simple patterns now, must be improved in future (prefer to use simple parser)
        // TODO: 6 (of type) section may contains wrong combination of "*&const" it must be fixed
        // TODO: for 5 (of type) required recursive parsing to detect nested types with templates

        const QString type = "(?:(const)\\s+)?"                                               // const
                             "((?:\\w*:{2,})*)"                                               // namespaces
                             "(\\w+)"                                                         // typename
                             "(?:\\s*<\\s*((?:\\w+(?:\\w+:{2,})*(?:\\s*,\\s*)?)+)\\s*>\\s*)?" // template args
                             "\\s+([\\*\\s\\&const]*)";                                       // &*const
        // see description of "type" regex
        const QString highLvlType = "(?:const\\s+)?"
                                    "(?:(?:\\w*:{2,})*)"
                                    "(?:\\w+)"
                                    "(?:\\s*<\\s*(?:(?:\\w+(?:\\w+:{2,})*(?:\\s*,\\s*)?)+)\\s*>\\s*)?"
                                    "\\s+(?:[\\*\\s\\&const]*)";

        const QString fieldPattern = "^(?:(volatile|static|mutable)\\s)?"  // 1 -- lhs keywords
                                     + type +                              // { type:
                                                                           //       2 -- const
                                                                           //       3 -- namespaces
                                                                           //       4 -- typename
                                                                           //       5 -- template args
                                                                           //       6 -- &*const
                                                                           // }
                                     "\\s*(\\w+)$";                        // 7 -- field name

        const QString methodPattern = "^(?:\\s*(static))?"                                            // 1 -- static
                                      "(?:\\s+(" + highLvlType + ")"                                  // 2 -- return type
                                      "(?:\\s+(\\w+))"                                                // 3 -- method name
                                      "(?:\\s*(\\((?:\\s*"+ highLvlType +"\\s*,\\s*)*\\)))"           // 4-- method arguments
                                      "(?:\\s+(const))?"                                              // 5 -- const
                                      "(?:\\s+((?:final|override)|(?:=\\s+default|delete|0))\\s*)?$"; // 6 -- lhs

        const QString propertyPattern = "^(\\w+)\\s+"                         // 1 -- type
                                        "(\\w+)"                              // 2 -- name
                                        "(?:\\s+(?:MEMBER)\\s+(\\w+))?"       // 3 -- member
                                        "(?:\\s+(?:READ)\\s+(\\w+))?"         // 4 -- getter
                                        "(?:\\s+(?:WRITE)\\s+(\\w+))?"        // 5 -- setter
                                        "(?:\\s+(?:RESET)\\s+(\\w+))?"        // 6 -- resetter
                                        "(?:\\s+(?:NOTIFY)\\s+(\\w+))?"       // 7 -- notifier
                                        "(?:\\s+(?:REVISION)\\s+(\\d+))?"     // 8 -- revision
                                        "(?:\\s+(?:DESIGNABLE)\\s+(\\w+))?"   // 9 -- designable
                                        "(?:\\s+(?:SCRIPTABLE)\\s+(\\w+))?"   // 10 -- scriptable
                                        "(?:\\s+(?:STORED)\\s+(true|false))?" // 11 -- stored
                                        "(?:\\s+(?:USER)\\s+(true|false))?"   // 12 -- user
                                        "(?:\\s+(CONSTANT))?"                 // 13 -- constant
                                        "(?:\\s+(FINAL))?$";                  // 14 -- final

        const QMap<models::DisplayPart, QString> componentPatternMap =
        {
            {models::DisplayPart::Fields, fieldPattern},
            {models::DisplayPart::Methods, methodPattern},
            {models::DisplayPart::Properties, propertyPattern},
        };

        const QMap<models::DisplayPart, int> componentsGroupCount =
        {
            {models::DisplayPart::Fields, int(FieldGroupNames::GroupsCount)},
            {models::DisplayPart::Methods, int(MethodsGroupsNames::GroupsCount)},
            {models::DisplayPart::Properties, int(PropGroupNames::GroupsCount)},
        };

        // Capture index, keywords which MUST NOT contains in captured text
        using NumberKeywords = std::pair<int, Keywords>;
        using Forbidden = QVector<NumberKeywords>;

        QMap<models::DisplayPart, Forbidden> forbiddenMap = {
            {models::DisplayPart::Fields,
            {
                {int(FieldGroupNames::Namespaces), reservedKeywords|types|boolKeywords},
                {int(FieldGroupNames::Typename), reservedKeywords|boolKeywords},
                {int(FieldGroupNames::Name), reservedKeywords|types|boolKeywords},
                {int(FieldGroupNames::TemplateArgs), reservedKeywords}
            }
            },
            {models::DisplayPart::Methods,
             {
                 {int(MethodsGroupsNames::ReturnType), reservedKeywords},
                 {int(MethodsGroupsNames::Name), reservedKeywords|types|boolKeywords},
                 {int(MethodsGroupsNames::Arguments), reservedKeywords|types|boolKeywords},
             }
            },
            {models::DisplayPart::Properties,
            {
                {int(PropGroupNames::Type), reservedKeywords|boolKeywords|propKeywords},
                {int(PropGroupNames::Name), reservedKeywords|types|boolKeywords|propKeywords},
                {int(PropGroupNames::Member), reservedKeywords|types|boolKeywords|propKeywords},
                {int(PropGroupNames::Getter), reservedKeywords|types|boolKeywords|propKeywords},
                {int(PropGroupNames::Setter), reservedKeywords|types|boolKeywords|propKeywords},
                {int(PropGroupNames::Resetter), reservedKeywords|types|boolKeywords|propKeywords},
                {int(PropGroupNames::Notifier), reservedKeywords|types|boolKeywords|propKeywords},
                {int(PropGroupNames::Designable), reservedKeywords|types|propKeywords},
                {int(PropGroupNames::Scriptable), reservedKeywords|types|propKeywords}
            }
            }
        };

        using RulesFunc = std::function<bool(const QString &, const Keywords &, SharedToken &)>;
        using GroupRules = QPair<int, RulesFunc>;
        using RulesMap = QMap<models::DisplayPart, GroupRules>;
        RulesMap rulesMap =
        {
            {models::DisplayPart::Methods,
                {int(MethodsGroupsNames::Arguments),
                    [](const QString &s, const Keywords &/*k*/, SharedToken &/*out*/){
                        const QStringList &types = s.split(",", QString::SkipEmptyParts);
                        if (types.isEmpty())
                            return true;

                        const QRegularExpression re(type);
                        for (auto &&t : types) {
                            auto match = re.match(t.trimmed());
                            if (!match.hasMatch()) // TODO: add checking keywords
                                return false;
                        }


                        return true;
                    }
                },
            },
        };

        bool split(const QRegularExpressionMatch &match, models::DisplayPart display, Tokens &out)
        {
            const int groupsCount = int(componentsGroupCount[display]);
            out.resize(groupsCount);

            const Forbidden &forbidden = forbiddenMap[display];
            for (int groupIndex = 1; groupIndex < groupsCount; ++groupIndex)
            {
                QString cap = match.captured(groupIndex).trimmed();
                out[groupIndex] = std::make_shared<Token>(cap);

                auto it = utility::find_if(forbidden, [&](auto &&c){ return c.first == groupIndex; });
                if (it != cend(forbidden)) {
                    const QStringList &tmpList =
                        cap.remove(QChar::Space).split(QRegExp("::|,"), QString::SkipEmptyParts);

                    if (!(tmpList.toSet() & it->second).isEmpty()) {
                        out.clear();
                        return false;
                    }
                }
            }

            return true;
        }
    }

    /**
     * @brief ComponentSignatureParser::parse
     * @param signature
     * @param display
     * @return
     */
    bool ComponentSignatureParser::parse(const QString &signature, models::DisplayPart display)
    {
        m_Tokens.clear();

        const auto &pattern = componentPatternMap[display];
        if (pattern.isEmpty())
            return false;

        const QRegularExpression re(pattern);
        const auto &match = re.match(signature.trimmed());
        if (match.hasMatch())
            return split(match, display, m_Tokens);

        return false;
    }

    /**
     * @brief ComponentSignatureParser::tokens
     * @return
     */
    Tokens ComponentSignatureParser::tokens() const
    {
        return m_Tokens;
    }

} // namespace components

