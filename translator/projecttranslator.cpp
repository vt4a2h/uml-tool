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

#include "projecttranslator.h"
#include "enums.h"
#include "templates.cpp"
#include "constants.cpp"
#include "code.h"

#include <QRegularExpression>

#include <db/database.h>
#include <db/projectdatabase.h>
#include <entity/enum.h>
#include <entity/extendedtype.h>
#include <entity/scope.h>
#include <entity/field.h>
#include <entity/classmethod.h>
#include <entity/union.h>
#include <entity/class.h>
#include <entity/templateclassmethod.h>
#include <entity/templateclass.h>
#include <utility/helpfunctions.h>

namespace {

    void addNamesapceHelper(QString &code, const QStringList &scopesNames, const QString &indent, QString scopeTemplate)
    {
        if (!code.isEmpty()) {
            if (!indent.isEmpty()) {
                code.prepend(indent);
                code.replace(QRegularExpression("(\n)(.)"), "\\1" + indent + "\\2");
            }

            code = scopeTemplate.replace("%name%", scopesNames.front())
                                .replace("%code%", code);
            code.append(" // namespace " + scopesNames.front());
        }
    }

}

namespace translator {

    /**
     * @brief ProjectTranslator::ProjectTranslator
     */
    ProjectTranslator::ProjectTranslator()
        : ProjectTranslator(nullptr, nullptr)
    {}

    /**
     * @brief ProjectTranslator::ProjectTranslator
     * @param globalDb
     * @param projectDb
     */
    ProjectTranslator::ProjectTranslator(const db::SharedDatabase &globalDb, const db::SharedDatabase &projectDb)
        : m_GlobalDatabase(globalDb)
        , m_ProjectDatabase(projectDb)
    {
    }

    /**
     * @brief ProjectTranslator::checkDb
     */
    void ProjectTranslator::checkDb() const
    {
        Q_ASSERT_X(m_GlobalDatabase, "ProjectTranslator", "global database not found");
        Q_ASSERT_X(m_ProjectDatabase, "ProjectTranslator", "project database not found");
    }

    /**
     * @brief ProjectTranslator::generateCodeForExtTypeOrType
     * @param id
     * @param options
     * @param localeDatabase
     * @param classDatabase
     * @return
     */
    QString ProjectTranslator::generateCodeForExtTypeOrType(const QString &id,
                                                            const TranslatorOptions &options,
                                                            const db::SharedDatabase &localeDatabase,
                                                            const db::SharedDatabase &classDatabase) const
    {
        checkDb();
        auto t = utility::findType(id, localeDatabase, classDatabase, m_ProjectDatabase, m_GlobalDatabase);
        if (!t)
            return "";

        entity::SharedExtendedType st = nullptr;
        if (t->hashType() == entity::ExtendedType::staticHashType())
            st = std::dynamic_pointer_cast<entity::ExtendedType>(t);

        return (st ? translate(st, options, localeDatabase, classDatabase)
                     .toHeader.append(st->isLink() || st->isPointer() ? "" : " ") :
                        t ? translate(t, options, localeDatabase, classDatabase).toHeader.append(" ") :
                            "");
    }

    /**
     * @brief ProjectTranslator::generateClassSection
     * @param _class
     * @param localeDatabase
     * @param section
     * @param out
     */
    void ProjectTranslator::generateClassSection(const entity::SharedClass &_class, const db::SharedDatabase &localeDatabase,
                                                 entity::Section section, QString &out) const
    {
        if (!_class->containsMethods(section) && !_class->containsFields(section)) return;

        out.append("\n");
        if (_class->kind() == entity::ClassType ||
            (_class->kind() != entity::StructType && section != entity::Public)) {
            out.append(INDENT)
               .append(utility::sectionToString(section))
               .append(":\n");
            generateFieldsAndMethods(_class, localeDatabase, DOUBLE_INDENT, section, out);
        } else {
            generateFieldsAndMethods(_class, localeDatabase, INDENT, section, out);
        }
    }

    /**
     * @brief ProjectTranslator::generateFieldsAndMethods
     * @param _class
     * @param localeDatabase
     * @param indent
     * @param section
     * @param out
     */
    void ProjectTranslator::generateFieldsAndMethods(const entity::SharedClass &_class,
                                                     const db::SharedDatabase &localeDatabase,
                                                     const QString &indent, entity::Section section,
                                                     QString &out) const
    {
        QStringList methodsList;
        for (auto &&method : _class->methods(section))
            methodsList << translate(method, WithNamespace, localeDatabase).toHeader.prepend(indent);
        out.append(methodsList.join(";\n"));
        if (!methodsList.isEmpty())
           out.append(";\n");

        QStringList fieldsList;
        for (auto &&field : _class->fields(section)) {
            auto t = utility::findType(field->typeId(), localeDatabase,
                                       m_GlobalDatabase, m_ProjectDatabase);
            if (!t)
               break;

            fieldsList << translate(field,
                                    t->scopeId() == _class->scopeId() ? NoOptions : WithNamespace,
                                    localeDatabase)
                          .toHeader.prepend(indent);
        }
        out.append(fieldsList.join(";\n"));

        if (!fieldsList.isEmpty())
           out.append(";\n");
    }

    /**
     * @brief ProjectTranslator::generateTemplatePart
     * @param result
     * @param t
     * @param withDefaultTypes
     */
    void ProjectTranslator::generateTemplatePart(QString &result, const entity::SharedTemplate &t, bool withDefaultTypes) const
    {
        if (!t)
           return;

        result.prepend(TEMPLATE_TEMPLATE + "\n");
        QStringList parameters;
        for (auto &&parameter : t->templateParameters()) {
            parameters << generateCodeForExtTypeOrType(parameter.first,
                                                       false,
                                                       t->database())
                          .prepend("class ")
                          .trimmed();
            if (!parameter.second.isEmpty() && parameter.second != STUB_ID && withDefaultTypes) {
                parameters.last()
                          .append(" = ")
                          .append(generateCodeForExtTypeOrType(parameter.second,
                                                               WithNamespace,
                                                               t->database()));
                parameters.last() = parameters.last().trimmed();
            }
        }
        result.replace("%template_parameters%", parameters.join(", "));
    }

    /**
     * @brief ProjectTranslator::toHeader
     * @param m
     * @param classDatabase
     * @return
     */
    bool ProjectTranslator::toHeader(const entity::SharedMethod &m,
                                     const db::SharedDatabase &classDatabase) const
    {
        if (m->type() == entity::TemplateMethod)
           return true;
        if (!classDatabase)
           return false;

        entity::FieldsList fields(m->parameters());
        auto it = std::find_if(fields.begin(), fields.end(), [&](const entity::SharedField &f) {
            return classDatabase->depthTypeSearch(f->typeId()) /*!= nullptr*/;
        });

        if (it != fields.end() || classDatabase->depthTypeSearch(m->returnTypeId()) /*!= nullptr*/)
           return true;

        return false;
    }

    /**
     * @brief ProjectTranslator::translate
     * @param _enum
     * @param options
     * @param localeDatabase
     * @param classDatabase
     * @return
     */
    Code ProjectTranslator::translate(const entity::SharedEnum &_enum,
                                      const TranslatorOptions  &options,
                                      const db::SharedDatabase &localeDatabase,
                                      const db::SharedDatabase &classDatabase) const
    {
        // compatibility with API
        Q_UNUSED(localeDatabase)
        Q_UNUSED(classDatabase)

        if (!_enum)
            return Code("\ninvalid enum\n", "");
        checkDb();

        QString result(ENUM_TEMPLATE);

        result.replace("%class%", _enum->isStrong() ? "class " : "");
        result.replace("%name%",  _enum->name());

        QString typeName("");
        QString typeId(_enum->enumTypeId());
        if (typeId != STUB_ID) {
            auto t = utility::findType(typeId, m_GlobalDatabase, m_ProjectDatabase);
            if (t)  typeName.append(" : ").append(t->name());
        }
        result.replace("%type%", typeName);

        QStringList values;
        if (options & GenerateNumbers) {
            for (auto &&v : _enum->variables())
                values << QString("%1 = %2").arg(v.first, QString::number(v.second));
        } else {
            for (auto &&v : _enum->variables())
                values << v.first;
        }
        result.replace("%values%", values.isEmpty() ? "" : values.join(", "));

        return Code(result, "");
    }

    /**
     * @brief ProjectTranslator::translate
     * @param method
     * @param options
     * @param localeDatabase
     * @param classDatabase
     * @return
     */
    Code ProjectTranslator::translate(const entity::SharedMethod &method,
                                      const TranslatorOptions  &options,
                                      const db::SharedDatabase &localeDatabase,
                                      const db::SharedDatabase &classDatabase) const
    {
        // compatibility with API
        Q_UNUSED(classDatabase)

        if (!method)
            return Code("\ninvalid method\n", "");
        checkDb();

        QString result(METHOD_TEMPLATE);

        entity::SharedTemplateClassMethod m(nullptr);
        if (method->type() == entity::TemplateMethod) {
            m = std::dynamic_pointer_cast<entity::TemplateClassMethod>(method);
            if (m)
                generateTemplatePart(result, std::static_pointer_cast<entity::Template>(m));
        }

        QString lhsIds("");
        if (!(options & NoLhs)) {
            QStringList lhsIdsList;
            for (auto &&lhsId : method->lhsIdentificators())
                lhsIdsList << utility::methodLhsIdToString(lhsId);
            if (!lhsIdsList.isEmpty())
                lhsIds.append(lhsIdsList.join(" ")).append(" ");
        }
        result.replace("%lhs_k%", lhsIds);

        result.replace("%r_type%", generateCodeForExtTypeOrType(method->returnTypeId(),
                                                                options,
                                                                m ? m->database() : nullptr,
                                                                localeDatabase));

        result.replace("%name%", method->name());

        QString parameters("");
        QStringList parametersList;
        for (auto &&p : method->parameters()) {
            p->removePrefix();
            p->removeSuffix();

            TranslatorOptions newOptions(options & NoDefaultName ? NoDefaultName : NoOptions);
            auto t = utility::findType(p->typeId(), localeDatabase,
                                       m_GlobalDatabase, m_ProjectDatabase);
            if (!t || method->scopeId() != t->scopeId() || method->scopeId() == STUB_ID)
               newOptions |= WithNamespace;

            parametersList << translate(p,
                                        newOptions,
                                        m ? m->database() : nullptr,
                                        localeDatabase).toHeader;
        }
        if (!parametersList.isEmpty())
            parameters.append(parametersList.join(", "));
        result.replace("%parameters%", parameters);

        QString rhsId(utility::methodRhsIdToString(method->rhsIdentificator()));
        if (method->rhsIdentificator() != entity::None)
            rhsId.prepend(" ");
        result.replace("%rhs_k%", rhsId);

        result.replace("%const%", method->isConst() ? " const" : "");

        return Code(result, "");
    }

    /**
     * @brief ProjectTranslator::translate
     * @param _union
     * @param options
     * @param localeDatabase
     * @param classDatabase
     * @return
     */
    Code ProjectTranslator::translate(const entity::SharedUnion &_union,
                                      const TranslatorOptions &options,
                                      const db::SharedDatabase &localeDatabase,
                                      const db::SharedDatabase &classDatabase) const
    {
        // compatibility with API
        Q_UNUSED(options)
        Q_UNUSED(localeDatabase)
        Q_UNUSED(classDatabase)

        if (!_union)
            return Code("\ninvalid union\n", "");
        checkDb();

        QString result(UNION_TEMPLATE);

        result.replace("%name%", _union->name());

        QStringList fields;
        for (auto &&field : _union->fields())
            fields << translate(field).toHeader.prepend(INDENT);

        QString resultFields(fields.join(";\n"));
        if (!resultFields.isEmpty())
            resultFields.append(";\n").prepend("\n");

        result.replace("%variables%", resultFields);

        return Code(result, "");
    }

    /**
     * @brief ProjectTranslator::translate
     * @param _class
     * @param options
     * @param localeDatabase
     * @param classDatabase
     * @return
     */
    Code ProjectTranslator::translate(const entity::SharedClass &_class,
                                      const TranslatorOptions &options,
                                      const db::SharedDatabase &localeDatabase,
                                      const db::SharedDatabase &classDatabase) const
    {
        // compatibility with API
        Q_UNUSED(options)
        Q_UNUSED(localeDatabase)
        Q_UNUSED(classDatabase)

        if (!_class)
            return Code("\ninvalid class\n", "");
        checkDb();

        QString result(CLASS_TEMPLATE);

        entity::SharedTemplateClass tc(nullptr);
        if (_class->hashType() == entity::TemplateClass::staticHashType()) {
            tc = std::dynamic_pointer_cast<entity::TemplateClass>(_class);
            if (tc)
                generateTemplatePart(result, std::static_pointer_cast<entity::Template>(tc));
        }

        result.replace("%kind%", _class->kind() == entity::ClassType ? "class " : "struct ");

        result.replace("%name%", _class->name()/*.append(" ")*/);

        QString parents("");
        if (_class->anyParents()) {
            QStringList parentsList;
            QString pString;
            entity::SharedType t(nullptr);
            for (auto &&p : _class->parents()) {
                t = utility::findType(p.first,
                                      tc ? tc->database() : nullptr,
                                      m_GlobalDatabase, m_ProjectDatabase);
                pString.append(utility::sectionToString(p.second))
                       .append(" ")
                       .append(t ?
                                   translate(t,
                                             t->scopeId() == _class->scopeId() ? NoOptions : WithNamespace,
                                             tc ? tc->database() : nullptr).toHeader :
                                   "unknown type");
                parentsList << pString;
                pString.clear();
            }
            parents.append(" : ")
                   .append(parentsList.join(", "))
                   .append(!parentsList.isEmpty() ? " " : "");
        }
        if (_class->kind() == entity::ClassType &&
           (_class->anyFields() || _class->anyMethods())) parents.append("\n");
        result.replace("%parents%", parents);

        QString section("");
        generateClassSection(_class, tc ? tc->database() : nullptr, entity::Public, section);
        generateClassSection(_class, tc ? tc->database() : nullptr, entity::Protected, section);
        generateClassSection(_class, tc ? tc->database() : nullptr, entity::Private, section);
        result.replace("%section%", section);

        if (section.isEmpty() && parents.isEmpty())
           result.replace(_class->name(), _class->name() + " ");

        return Code(result, "");
    }

    /**
     * @brief ProjectTranslator::translate
     * @param _class
     * @param options
     * @param localeDatabase
     * @param classDatabase
     * @return
     */
    Code ProjectTranslator::translate(const entity::SharedTemplateClass &_class,
                                      const TranslatorOptions &options,
                                      const db::SharedDatabase &localeDatabase,
                                      const db::SharedDatabase &classDatabase) const
    {
        return translate(std::static_pointer_cast<entity::Class>(_class), options, localeDatabase,
                         classDatabase);
    }

    /**
     * @brief ProjectTranslator::generateClassMethodsImpl
     * @param _class
     * @param localeDatabase
     * @return
     */
    Code ProjectTranslator::generateClassMethodsImpl(const entity::SharedClass &_class,
                                                     const db::SharedDatabase &localeDatabase) const
    {
        if (!_class)
            return Code("\ninvalid class\n", "\ninvalid class\n");
        checkDb();

        QStringList methodsCpp;
        QStringList methodsH;
        QString method;

        entity::SharedTemplateClass tc(std::dynamic_pointer_cast<entity::TemplateClass>(_class));
        QString templatePart;
        QString methodTemplatePart;
        QString newName;

        if (tc) {
           generateTemplatePart(templatePart, tc, false);

           methodTemplatePart = templatePart;
           methodTemplatePart.remove("template ").remove("class ").remove("\n");

           newName = _class->name().append(methodTemplatePart);
        }

        for (auto &&m : _class->methods()) {
            method = translate(m, NoLhs | WithNamespace | NoDefaultName, localeDatabase).toHeader;
            if (tc)
                method.prepend(templatePart);

            method.replace(m->name(), m->name().prepend(_class->name().append("::")));
            method.append("\n{\n}\n");

            if (tc)
                method.replace(method.indexOf(_class->name()), _class->name().size(), newName);

            (toHeader(m, tc ? tc->database() : nullptr) || tc ? methodsH : methodsCpp) << method;
            method.clear();
        }

        if (tc && !methodsH.isEmpty()) {
           methodsH.last().remove(methodsH.last().size() - 1, 1);
           methodsH.first().prepend("\n");
        }

        return Code(methodsH.join("\n"), methodsCpp.join("\n"));
    }

    /**
     * @brief ProjectTranslator::generateClassMethodsImpl
     * @param _class
     * @return
     */
    Code ProjectTranslator::generateClassMethodsImpl(const entity::SharedTemplateClass &_class) const
    {
        if (!_class)
           return Code("\ninvalid template class\n", "\ninvalid template class\n");
        checkDb();

        return generateClassMethodsImpl(std::dynamic_pointer_cast<entity::Class>(_class),
                                        _class->database());
    }

    /**
     * @brief ProjectTranslator::addNamespace
     * @param type
     * @param code
     * @param indentCount
     */
    void ProjectTranslator::addNamespace(const entity::SharedType &type, Code &code, uint indentCount)
    {
        if (!type || !m_ProjectDatabase)
            return;

        QString newIndent;
        while (indentCount != 0) {
            newIndent.append(INDENT);
            --indentCount;
        }

        QStringList scopesNames(utility::scopesNamesList(type,m_ProjectDatabase));
        while (!scopesNames.isEmpty()) {
            addNamesapceHelper(code.toHeader, scopesNames, newIndent, SCOPE_TEMPLATE_HEADER);
            addNamesapceHelper(code.toSource, scopesNames, newIndent, SCOPE_TEMPLATE_SOURCE);
            scopesNames.pop_front();
        }
    }

    /**
     * @brief ProjectTranslator::translate
     * @param extType
     * @param options
     * @param localeDatabase
     * @param classDatabase
     * @return
     */
    Code ProjectTranslator::translate(const entity::SharedExtendedType &extType,
                                      const TranslatorOptions &options,
                                      const db::SharedDatabase &localeDatabase,
                                      const db::SharedDatabase &classDatabase) const
    {
        if (!extType)
            return Code("\ninvalid extended type\n", "");
        checkDb();
        QString result(EXT_TYPE_TEMPLATE);

        result.replace("%const%", extType->isConst() ? "const " : "");

        if (extType->typeId() != STUB_ID) {
            auto t = utility::findType(extType->typeId(), localeDatabase, classDatabase,
                                       m_ProjectDatabase,
                                       m_GlobalDatabase);
            result.replace("%name%", t ?
                this->translate(t, options, localeDatabase, classDatabase).toHeader : "");
        } else
            result.remove("%name%");

        QString pl("");
        if (!extType->pl().isEmpty()) {
            for (auto &&c : extType->pl()) {
                pl.append(c.first);
                if (c.second) pl.append(" const ");
            }
            pl = pl.trimmed();
            pl.prepend(" ");
        }
        result.replace("%pl%", pl);

        QString params("");
        if (!extType->templateParameters().isEmpty()) {
            QStringList names;
            entity::SharedType t = nullptr;
            for (auto &&id : extType->templateParameters()) {
                t = utility::findType(id, localeDatabase, classDatabase, m_ProjectDatabase,
                                      m_GlobalDatabase);
                if (t) names << t->name();
            }
            params = names.join(", ");
            params.append(">");
            params.prepend("<");
        }
        result.replace("%template_params%", params);

        if ((options & WithAlias) && extType->name() != DEFAULT_NAME)
            result.prepend(QString("using %1 = ").arg(extType->name())).append(";");

        return Code(result, "");
    }

    /**
     * @brief ProjectTranslator::translate
     * @param field
     * @param options
     * @param localeDatabase
     * @param classDatabase
     * @return
     */
    Code ProjectTranslator::translate(const entity::SharedField &field,
                                      const TranslatorOptions  &options,
                                      const db::SharedDatabase &localeDatabase,
                                      const db::SharedDatabase &classDatabase) const
    {
        if (!field)
            return Code("\ninvalid field\n", "");
        checkDb();
        QString result(FIELD_TEMPLATE);

        QStringList keywords;
        if (!field->keywords().isEmpty())
            for (auto &&keyword : field->keywords())
                keywords << utility::fieldKeywordToString(keyword);
        result.replace("%keywords%", keywords.isEmpty() ? "" : keywords.join(" ").append(" "));

        result.replace("%type%", generateCodeForExtTypeOrType(field->typeId(),
                                                              options,
                                                              localeDatabase,
                                                              classDatabase));
        result.replace("%name%", field->fullName());

        if (!field->defaultValue().isEmpty() && !(options & NoDefaultName))
            result.append( " = " + field->defaultValue() );

        return Code(result, "");
    }

    /**
     * @brief ProjectTranslator::projectDatabase
     * @return
     */
    db::SharedDatabase ProjectTranslator::projectDatabase() const
    {
        return m_ProjectDatabase;
    }

    /**
     * @brief ProjectTranslator::setProjectDatabase
     * @param projectDatabase
     */
    void ProjectTranslator::setProjectDatabase(const db::SharedDatabase &projectDatabase)
    {
        m_ProjectDatabase = projectDatabase;
    }

    /**
     * @brief ProjectTranslator::translate
     * @param type
     * @param options
     * @param localeDatabase
     * @param classDatabase
     * @return
     */
    Code ProjectTranslator::translate(const entity::SharedType &type, const TranslatorOptions &options,
                                      const db::SharedDatabase &localeDatabase, const db::SharedDatabase &classDatabase) const
    {
        QStringList scopesNames;
        QString id = type->scopeId();
        entity::SharedScope scope = utility::findScope(id, localeDatabase, classDatabase, m_ProjectDatabase,
                                                       m_GlobalDatabase);
        while (id != GLOBAL_SCOPE_ID || id != LOCALE_TEMPLATE_SCOPE_ID) {
            if (!scope)
                break;
            if (scope->name() != DEFAULT_NAME)
                scopesNames.prepend(scope->name());
            id = scope->parentScopeId();
            scope = utility::findScope(id, localeDatabase, classDatabase, m_ProjectDatabase, m_GlobalDatabase);
        }

        if (!scopesNames.isEmpty() && (options & WithNamespace)) {
            scopesNames << type->name();
            return Code(scopesNames.join("::"), "");
        }

        return Code(type->name(), "");
    }

    /**
     * @brief ProjectTranslator::globalDatabase
     * @return
     */
    db::SharedDatabase ProjectTranslator::globalDatabase() const
    {
        return m_GlobalDatabase;
    }

    /**
     * @brief ProjectTranslator::setGlobalDatabase
     * @param globalDatabase
     */
    void ProjectTranslator::setGlobalDatabase(const db::SharedDatabase &globalDatabase)
    {
        m_GlobalDatabase = globalDatabase;
    }

} // namespace translator
