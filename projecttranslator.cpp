#include "projecttranslator.h"
#include "enum.h"
#include "enums.h"
#include "extendedtype.h"
#include "database.h"
#include "projectdatabase.h"
#include "scope.h"
#include "field.h"
#include "classmethod.h"
#include "union.h"
#include "class.h"
#include "helpfunctions.h"
#include "templateclassmethod.h"
#include "templateclass.h"
#include "templates.cpp"
#include "constants.cpp"

namespace translator {

    ProjectTranslator::ProjectTranslator()
        : ProjectTranslator(nullptr, nullptr)
    {}

    ProjectTranslator::ProjectTranslator(const db::SharedDatabase &globalDb, const db::SharedDatabase &projectDb)
        : m_GlobalDatabase(globalDb)
        , m_ProjectDatabase(projectDb)
    {}

    void ProjectTranslator::checkDb() const
    {
        Q_ASSERT_X(m_GlobalDatabase, "ProjectTranslator", "global database not found");
        Q_ASSERT_X(m_ProjectDatabase, "ProjectTranslator", "project database not found");
    }

    QString ProjectTranslator::generateCodeForExtTypeOrType(const QString &id,
                                                            bool withNamespace,
                                                            const db::SharedDatabase &localeDatabase,
                                                            const db::SharedDatabase &classDatabase) const
    {
        checkDb();
        auto t = utility::findType(id, localeDatabase, classDatabase, m_ProjectDatabase, m_GlobalDatabase);
        if (!t) return "";

        entity::SharedExtendedType st = nullptr;
        if (t->type() == entity::ExtendedTypeType)
            st = std::dynamic_pointer_cast<entity::ExtendedType>(t);

        return (st ? generateCode(st, withNamespace, localeDatabase, classDatabase)
                     .append(st->isLink() || st->isPointer() ? "" : " ") :
                        t ? generateCode(t, withNamespace, localeDatabase, classDatabase).append(" ") :
                            "");
    }

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

    void ProjectTranslator::generateFieldsAndMethods(const entity::SharedClass &_class,
                                                     const db::SharedDatabase &localeDatabase,
                                                     const QString &indent, entity::Section section,
                                                     QString &out) const
    {
        QStringList methodsList;
        for (auto &&method : _class->methods(section))
            methodsList << generateCode(method, localeDatabase).prepend(indent);
        out.append(methodsList.join(";\n"));
        if (!methodsList.isEmpty()) out.append(";\n");

        QStringList fieldsList;
        for (auto &&field : _class->fields(section)) {
            auto t = utility::findType(field->typeId(), localeDatabase,
                                       m_GlobalDatabase, m_ProjectDatabase);
            if (!t) break;
            fieldsList << generateCode(field,
                                       t->scopeId() == _class->scopeId() ? false : true,
                                       localeDatabase)
                          .prepend(indent);
        }
        out.append(fieldsList.join(";\n"));

        if (!fieldsList.isEmpty()) out.append(";\n");
    }

    void ProjectTranslator::generateTemplatePart(QString &result, const entity::SharedTemplate &t, bool withDefaultTypes) const
    {
        if (!t) return;

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
                                                               true,
                                                               t->database()));
                parameters.last() = parameters.last().trimmed();
            }
        }
        result.replace("%template_parameters%", parameters.join(", "));
    }

    bool ProjectTranslator::toHeader(const entity::SharedMethod &m,
                                     const db::SharedDatabase &classDatabase) const
    {
        if (m->type() == entity::TemplateMethod) return true;
        if (!classDatabase) return false;

        entity::FieldsList fields(m->parameters());
        auto it = std::find_if(fields.begin(), fields.end(),
                               [&](const entity::SharedField &f) {
            return classDatabase->depthTypeSearch(f->typeId()) != nullptr;
        });

        if (it != fields.end() ||
            classDatabase->depthTypeSearch(m->returnTypeId()) != nullptr) return true;

        return false;
    }

    QString ProjectTranslator::generateCode(const entity::SharedEnum &_enum, bool generateNumbers) const
    {
        if (!_enum) return "\ninvalid enum\n";
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
        if (generateNumbers) {
            for (auto &&v : _enum->variables())
                values << QString("%1 = %2").arg(v.first, QString::number(v.second));
        } else {
            for (auto &&v : _enum->variables())
                values << v.first;
        }
        result.replace("%values%", values.isEmpty() ? "" : values.join(", "));

        return result;
    }

    QString ProjectTranslator::generateCode(const entity::SharedMethod &method,
                                            const db::SharedDatabase &localeDatabase) const
    {
        if (!method) return "\ninvalid method\n";
        checkDb();

        QString result(METHOD_TEMPLATE);

        entity::SharedTemplateClassMethod m(nullptr);
        if (method->type() == entity::TemplateMethod) {
            m = std::dynamic_pointer_cast<entity::TemplateClassMethod>(method);
            if (m)
                generateTemplatePart(result, std::static_pointer_cast<entity::Template>(m));
        }

        QString lhsIds("");
        QStringList lhsIdsList;
        for (auto &&lhsId : method->lhsIdentificators())
            lhsIdsList << utility::methodLhsIdToString(lhsId);
        if (!lhsIdsList.isEmpty()) lhsIds.append(lhsIdsList.join(" ")).append(" ");
        result.replace("%lhs_k%", lhsIds);

        result.replace("%r_type%", generateCodeForExtTypeOrType(method->returnTypeId(),
                                                                true,
                                                                m ? m->database() : nullptr,
                                                                localeDatabase));

        result.replace("%name%", method->name());

        QString parameters("");
        QStringList parametersList;
        for (auto &&p : method->parameters()) {
            p->removePrefix();
            p->removeSuffix();
            parametersList << generateCode(p, true,
                                           m ? m->database() : nullptr,
                                           localeDatabase);
        }
        if (!parametersList.isEmpty()) parameters.append(parametersList.join(", "));
        result.replace("%parameters%", parameters);

        QString rhsId(utility::methodRhsIdToString(method->rhsIdentificator()));
        if (method->rhsIdentificator() != entity::None) rhsId.prepend(" ");
        result.replace("%rhs_k%", rhsId);

        result.replace("%const%", method->isConst() ? " const" : "");

        return result;
    }

    QString ProjectTranslator::generateCode(const entity::SharedUnion &_union) const
    {
        if (!_union) return "\ninvalid union\n";
        checkDb();
        QString result(UNION_TEMPLATE);

        result.replace("%name%", _union->name());

        QStringList fields;
        for (auto &&field : _union->fields())
            fields << generateCode(field).prepend(INDENT);

        QString resultFields(fields.join(";\n"));
        if (!resultFields.isEmpty()) resultFields.append(";\n").prepend("\n");

        result.replace("%variables%", resultFields);

        return result;
    }

    QString ProjectTranslator::generateCode(const entity::SharedClass &_class) const
    {
        if (!_class) return "\ninvalid class\n";
        checkDb();
        QString result(CLASS_TEMPLATE);

        entity::SharedTemplateClass tc(nullptr);
        if (_class->type() == entity::TemplateClassType) {
            tc = std::dynamic_pointer_cast<entity::TemplateClass>(_class);
            if (tc)
                generateTemplatePart(result, std::static_pointer_cast<entity::Template>(tc));
        }

        result.replace("%kind%", _class->kind() == entity::ClassType ? "class " : "struct ");

        result.replace("%name%", _class->name().append(" "));

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
                                   generateCode(t,
                                                t->scopeId() == _class->scopeId() ? false : true,
                                                tc ? tc->database() : nullptr) :
                                   "unknown type");
                parentsList << pString;
                pString.clear();
            }
            parents.append(": ").append(parentsList.join(", ")).append(" ");
        }
        if (_class->kind() == entity::ClassType &&
           (_class->anyFields() || _class->anyMethods())) parents.append("\n");
        result.replace("%parents%", parents);

        QString section("");
        generateClassSection(_class, tc ? tc->database() : nullptr, entity::Public, section);
        generateClassSection(_class, tc ? tc->database() : nullptr, entity::Protected, section);
        generateClassSection(_class, tc ? tc->database() : nullptr, entity::Private, section);
        result.replace("%section%", section);

        return result;
    }

    QString ProjectTranslator::generateCode(const entity::SharedTemplateClass &_class) const
    {
        return generateCode(std::dynamic_pointer_cast<entity::Class>(_class));
    }

    Code ProjectTranslator::generateClassMethodsImpl(const entity::SharedClass &_class,
                                                     const db::SharedDatabase &localeDatabase) const
    {
        if (!_class) return Code("\ninvalid class\n", "\ninvalid class\n");
        checkDb();

        QStringList methodsCpp;
        QStringList methodsH;
        QString method;
        entity::SharedTemplateClass tc(std::dynamic_pointer_cast<entity::TemplateClass>(_class));
        for (auto &&m : _class->methods()) {
            method = generateCode(m, localeDatabase);
            if (tc) method.prepend(TEMPLATE);
            method.replace(m->name(), m->name().prepend(_class->name().append("::")));
            method.append("\n{\n}").append("\n");

            (toHeader(m, tc ? tc->database() : nullptr) ? methodsH : methodsCpp) << method;
            method.clear();
        }

        return Code(methodsH.join("\n"), methodsCpp.join("\n"));
    }

    Code ProjectTranslator::generateClassMethodsImpl(const entity::SharedTemplateClass &_class) const
    {
        if (!_class) return Code("\ninvalid template class\n", "\ninvalid template class\n");
        checkDb();

        QString templatePart;
        generateTemplatePart(templatePart,
                             std::dynamic_pointer_cast<entity::Template>(_class),
                             false);
        QString methodTemplatePart(templatePart);
        methodTemplatePart.remove("template ").remove("class ").remove("\n");

        Code result(generateClassMethodsImpl(std::dynamic_pointer_cast<entity::Class>(_class), _class->database()));
        result.toHeader.replace(TEMPLATE, templatePart);
        result.toHeader.replace(_class->name(), _class->name().append(methodTemplatePart));

        return result;
    }

    QString ProjectTranslator::generateCode(const entity::SharedExtendedType &extType,
                                            bool withNamespace,
                                            const db::SharedDatabase &localeDatabase,
                                            const db::SharedDatabase &classDatabase,
                                            bool alias) const
    {
        if (!extType) return "\ninvalid extended type\n";
        checkDb();
        QString result(EXT_TYPE_TEMPLATE);

        result.replace("%const%", extType->isConst() ? "const " : "");

        if (extType->typeId() != STUB_ID) {
            auto t = utility::findType(extType->typeId(), localeDatabase, classDatabase,
                                       m_ProjectDatabase,
                                       m_GlobalDatabase);
            result.replace("%name%", t ? this->generateCode(t, withNamespace, localeDatabase, classDatabase) : "");
        } else {
            result.remove("%name%");
        }

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

        if (alias && extType->name() != DEFAULT_NAME)
            result.prepend(QString("using %1 = ").arg(extType->name())).append(";");

        return result;
    }

    QString ProjectTranslator::generateCode(const entity::SharedField &field, bool withNamespace,
                                            const db::SharedDatabase &localeDatabase,
                                            const db::SharedDatabase &classDatabase) const
    {
        if (!field) return "\ninvalid field\n";
        checkDb();
        QString result(FIELD_TEMPLATE);

        QStringList keywords;
        if (!field->keywords().isEmpty())
            for (auto &&keyword : field->keywords())
                keywords << utility::fieldKeywordToString(keyword);
        result.replace("%keywords%", keywords.isEmpty() ? "" : keywords.join(" ").append(" "));

        result.replace("%type%", generateCodeForExtTypeOrType(field->typeId(),
                                                              withNamespace,
                                                              localeDatabase,
                                                              classDatabase));
        result.replace("%name%", field->fullName());

        return result;
    }

    db::SharedDatabase ProjectTranslator::projectDatabase() const
    {
        return m_ProjectDatabase;
    }

    void ProjectTranslator::setProjectDatabase(const db::SharedDatabase &projectDatabase)
    {
        m_ProjectDatabase = projectDatabase;
    }

    QString ProjectTranslator::generateCode(const entity::SharedType &type, bool withNamespace,
                                            const db::SharedDatabase &localeDatabase, const db::SharedDatabase &classDatabase) const
    {
        QStringList scopesNames;
        QString id = type->scopeId();
        entity::SharedScope scope = utility::findScope(id, localeDatabase, classDatabase, m_ProjectDatabase,
                                                       m_GlobalDatabase);
        while (id != GLOBAL_SCOPE_ID || id != LOCALE_TEMPLATE_SCOPE_ID) {
            if (!scope) break;
            if (scope->name() != DEFAULT_NAME) scopesNames.prepend(scope->name());
            id = scope->parentScopeId();
            scope = utility::findScope(id, localeDatabase, classDatabase, m_ProjectDatabase, m_GlobalDatabase);
        }

        if (!scopesNames.isEmpty() && withNamespace) {
            scopesNames << type->name();
            return scopesNames.join("::");
        }

        return type->name();
    }

    db::SharedDatabase ProjectTranslator::globalDatabase() const
    {
        return m_GlobalDatabase;
    }

    void ProjectTranslator::setGlobalDatabase(const db::SharedDatabase &globalDatabase)
    {
        m_GlobalDatabase = globalDatabase;
    }

} // namespace translator
