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
#include <QDebug>

#include <range/v3/algorithm/find_if.hpp>

#include <Models/ComponentsModel.h>

#include <Utility/helpfunctions.h>

#include "token.h"
#include "componentscommon.h"

namespace Components {

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
                             "(?:\\s+((?:[\\*\\s\\&]|const)*))?";                             // &*const

        // unlike "type" regex, this one doesn't match anything
        const QString highLvlType = "(?:const\\s+)?"
                                    "(?:(?:\\w*:{2,})*)"
                                    "(?:\\w+)"
                                    "(?:\\s*<\\s*(?:(?:\\w+(?:\\w+:{2,})*(?:\\s*,\\s*)?)+)\\s*>\\s*)?"
                                    "(?:\\s+(?:(?:[\\*\\s\\&]|const)*))?";

        const QString argumentPattern = "(?:(" + highLvlType + "))" // 1 -- type
                                        "(?:\\s*(\\w*))";           // 2 -- name (is optional)

        const QString fieldPattern = "^(?:(volatile|static|mutable)\\s)?"  // 1 -- lhs keywords
                                     + type +                              // { type:
                                                                           //       2 -- const
                                                                           //       3 -- namespaces
                                                                           //       4 -- typename
                                                                           //       5 -- template args
                                                                           //       6 -- &*const
                                                                           // }
                                     "(?:\\s+(\\w+))$";                    // 7 -- field name

        const QString methodPattern =
            "^(?:\\s*(explicit|inline|static|virtual|friend)\\s+)?"             // 1 -- rhs
            "(?:(" + highLvlType + ")\\s+)"                                     // 2 -- return type
            "(?:(\\w+))"                                                        // 3 -- method name
            "(?:\\s*\\(((?:[\\w\\s\\*\\&:,_<>])*)\\))"                          // 4 -- method arguments
                                                                                //      raw matching
            "(?:\\s+(const))?"                                                  // 5 -- const
            "(?:\\s+((?:final|override)|(?:=\\s+(?:default|delete|0)))\\s*)?$"; // 6 -- lhs

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

        const QMap<Models::DisplayPart, QString> componentPatternMap =
        {
            {Models::DisplayPart::Fields, fieldPattern},
            {Models::DisplayPart::Methods, methodPattern},
            {Models::DisplayPart::Properties, propertyPattern},
        };

        const QMap<Models::DisplayPart, int> componentsGroupCount =
        {
            {Models::DisplayPart::Fields, int(FieldGroupNames::GroupsCount)},
            {Models::DisplayPart::Methods, int(MethodsGroupsNames::GroupsCount)},
            {Models::DisplayPart::Properties, int(PropGroupNames::GroupsCount)},
        };

        // Capture index, keywords which MUST NOT contains in captured text
        using NumberKeywords = std::pair<int, Keywords>;
        using Forbidden = QVector<NumberKeywords>;

        QMap<Models::DisplayPart, Forbidden> forbiddenMap = {
            {Models::DisplayPart::Fields,
            {
                {int(FieldGroupNames::Namespaces), reservedKeywords|types|boolKeywords},
                {int(FieldGroupNames::Typename), reservedKeywords|boolKeywords},
                {int(FieldGroupNames::Name), reservedKeywords|types|boolKeywords},
                {int(FieldGroupNames::TemplateArgs), reservedKeywords}
            }
            },
            {Models::DisplayPart::Methods,
             {
                 {int(MethodsGroupsNames::Name), reservedKeywords|types|boolKeywords},
                 {int(MethodsGroupsNames::Arguments), reservedKeywords|types|boolKeywords},
             }
            },
            {Models::DisplayPart::Properties,
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

        const QMap<int, Keywords> forbiddenForTypes =
        {
            {int(TypeGroups::Namespaces), types|boolKeywords|reservedKeywords},
            {int(TypeGroups::Typename), reservedKeywords|boolKeywords},
            {int(TypeGroups::TemplateArgs), boolKeywords},
        };

        bool parseType(const QString &s, Tokens &tokens)
        {
            const QRegularExpression re(type);
            auto match = re.match(s.trimmed());
            if (match.hasMatch()) {
                tokens.resize(int(TypeGroups::GroupsCount));
                for (int i = 1; i < int(TypeGroups::GroupsCount); ++i)
                {
                    const QString &cap = match.captured(i).trimmed();
                    tokens[i] = std::make_shared<Token>(cap);

                    const Keywords &forbidden = forbiddenForTypes.value(i);
                    if (!forbidden.isEmpty()) {
                        if (!(tokens[i]->token().split("::", QString::SkipEmptyParts)
                              .toSet() & forbidden).isEmpty()) {
                            tokens.clear();
                            return false;
                        }
                    }
                }
            }

            return true;
        }

        using RulesFunc = std::function<bool(const QString &, SharedToken &)>;
        using GroupRules = QPair<int, RulesFunc>;
        using GroupRulesVector = QVector<GroupRules>;
        using RulesMap = QMap<Models::DisplayPart, GroupRulesVector>;
        RulesMap rulesMap =
        {
            {Models::DisplayPart::Methods,
                {
                    {int(MethodsGroupsNames::ReturnType),
                     [](const QString &s, SharedToken &out){
                         if (s.trimmed().isEmpty()) {
                            out = std::make_shared<Token>();
                            return true;
                         }

                         Tokens tokens;
                         if (parseType(s, tokens)) {
                            out = std::make_shared<Token>(tokens);
                            return true;
                         }

                         return false;
                     }
                    },
                    {int(MethodsGroupsNames::Arguments),
                     [](const QString &s, SharedToken &out) {
                         QString trimmedIn = s.trimmed();

                        // It's OK if method has no arguments
                        if (trimmedIn.isEmpty()) {
                            out = std::make_shared<Token>();
                            return true;
                        }

                        QStringList arguments = s.split(",", QString::SkipEmptyParts);
                        Tokens tmpOut;
                        tmpOut.reserve(arguments.count());
                        for (auto &&arg : arguments) {
                            const QRegularExpression re(argumentPattern);
                            auto match = re.match(arg.trimmed());

                            if (match.hasMatch()) {
                               Tokens argTokens(int(Argument::GroupsCount));

                               Tokens type;
                               int typeIndex = int(Argument::Type);
                               if (parseType(match.captured(typeIndex), type))
                                   argTokens[typeIndex] = std::make_shared<Token>(type);
                               else {
                                   out = std::make_shared<Token>();
                                   return false;
                               }

                               int nameIndex = int(Argument::Name);
                               QString name = match.captured(nameIndex).trimmed();
                               argTokens[nameIndex] = name.isEmpty() ? std::make_shared<Token>()
                                                                     : std::make_shared<Token>(name);

                               tmpOut << std::make_shared<Token>(argTokens);
                            } else {
                               out = std::make_shared<Token>();
                               return false;
                            }
                        }

                        out = std::make_shared<Token>(tmpOut);
                        return true;
                     }
                    },
                },
            },
        };

        bool split(const QRegularExpressionMatch &match, Models::DisplayPart display, Tokens &out)
        {
            const int groupsCount = int(componentsGroupCount[display]);
            out.resize(groupsCount);

            for (int groupIndex = 1; groupIndex < groupsCount; ++groupIndex)
            {
                QString cap = match.captured(groupIndex).trimmed();
                out[groupIndex] = std::make_shared<Token>(cap);

                const Forbidden &forbidden = forbiddenMap[display];
                auto fIt = ranges::find_if(forbidden, [&](auto &&c){ return c.first == groupIndex; });

                const GroupRulesVector &rules = rulesMap[display];
                auto rIt = ranges::find_if(rules, [&](auto &&r){ return r.first == groupIndex; });

                // Check extra rules
                if (rIt != cend(rules)) {
                    bool result = rIt->second(cap, out[groupIndex]);
                    if (!result){
                        out.clear();
                        return false;
                    }
                }

                // Check forbidden words. Do not check forbidden words if there are some custom rules.
                if (fIt != cend(forbidden) && rIt == cend(rules)) {
                    const QStringList &tmpList =
                        cap.remove(QChar::Space).split(QRegExp("::|,"), QString::SkipEmptyParts);

                    if (!(tmpList.toSet() & fIt->second).isEmpty()) {
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
    bool ComponentSignatureParser::parse(const QString &signature, Models::DisplayPart display)
    {
        m_Tokens.clear();

        const auto &pattern = componentPatternMap[display];
        if (pattern.isEmpty())
            return false;

        const QRegularExpression re(pattern);
        if (re.isValid()) {
            const auto &match = re.match(signature.trimmed());
            if (match.hasMatch())
                return split(match, display, m_Tokens);
        } else {
            qWarning() << "In:" << Q_FUNC_INFO << "-- regexp is not valid:" << re.errorString()
                       << "at:" << re.patternErrorOffset();
        }

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

