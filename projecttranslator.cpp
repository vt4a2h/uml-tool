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
#include "helpfunctions.h"
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

    QString ProjectTranslator::generateCodeForExtTypeOrType(const QString &id) const
    {
        checkDb();

        auto t = utility::findType(m_GlobalDatabase, m_ProjectDatabase, id);
        if (!t) return "";

        entity::SharedExtendedType st = nullptr;
        if (t->type() == entity::ExtendedTypeType)
            st = std::dynamic_pointer_cast<entity::ExtendedType>(t);

        return (st ? generateCode(st)
                     .append(st->isLink() || st->isPointer() ? "" : " ") :
                        t ? generateCode(t).append(" ") :
                            "");
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
            auto t = utility::findType(m_GlobalDatabase, m_ProjectDatabase, typeId);
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

    QString ProjectTranslator::generateCode(const entity::SharedMethod &method) const
    {
        if (!method) return "\ninvalid method\n";
        checkDb();
        QString result(METHOD_TEMPLATE);

        QString lhsIds("");
        QStringList lhsIdsList;
        for (auto &&lhsId : method->lhsIdentificators())
            lhsIdsList << utility::methodLhsIdToString(lhsId);
        if (!lhsIdsList.isEmpty()) lhsIds.append(lhsIdsList.join(" ")).append(" ");
        result.replace("%lhs_k%", lhsIds);

        result.replace("%r_type%", generateCodeForExtTypeOrType(method->returnTypeId()));

        result.replace("%name%", method->name());

        QString parameters("");
        QStringList parametersList;
        for (auto &&p : method->parameters()) {
            p->removePrefix();
            p->removeSuffix();
            parametersList << generateCode(p);
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
        QString result(UNION_TEMPLATE);

        result.replace("%name%", _union->name());

        QStringList fields;
        for (auto &&field : _union->fields())
            fields << generateCode(field).prepend(INDENT);
        result.replace("%variables%", fields.join(";\n"));

        return result;
    }

    QString ProjectTranslator::generateCode(const entity::SharedExtendedType &extType, bool alias) const
    {
        if (!extType) return "\ninvalid extended type\n";
        checkDb();
        QString result(EXT_TYPE_TEMPLATE);

        result.replace("%const%", extType->isConst() ? "const " : "");

        if (extType->typeId() != STUB_ID) {
            auto t = utility::findType(m_GlobalDatabase, m_ProjectDatabase, extType->typeId());
            result.replace("%name%", t ? this->generateCode(t) : "");
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
                t = utility::findType(m_GlobalDatabase, m_ProjectDatabase, id);
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

    QString ProjectTranslator::generateCode(const entity::SharedField &field) const
    {
        if (!field) return "\ninvalid field\n";
        checkDb();
        QString result(FIELD_TEMPLATE);

        QStringList keywords;
        if (!field->keywords().isEmpty())
            for (auto &&keyword : field->keywords())
                keywords << utility::fieldKeywordToString(keyword);
        result.replace("%keywords%", keywords.isEmpty() ? "" : keywords.join(" ").append(" "));

        result.replace("%type%", generateCodeForExtTypeOrType(field->typeId()));
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

    QString ProjectTranslator::generateCode(const entity::SharedType &type) const
    {
        QStringList scopesNames;
        QString id = type->scopeId();
        entity::SharedScope scope = utility::findScope(m_GlobalDatabase, m_ProjectDatabase,
                                                       id);
        while (id != GLOBAL_SCOPE_ID) {
            if (!scope) break;
            if (scope->name() != DEFAULT_NAME) scopesNames.prepend(scope->name());
            id = scope->parentScopeId();
            scope = utility::findScope(m_GlobalDatabase, m_ProjectDatabase, id);
        }

        if (!scopesNames.isEmpty()) {
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
