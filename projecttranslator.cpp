#include "projecttranslator.h"
#include "enum.h"
#include "enums.h"
#include "extendedtype.h"
#include "database.h"
#include "projectdatabase.h"
#include "scope.h"
#include "field.h"
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

    QString ProjectTranslator::generateCode(const entity::SharedEnum &type, bool generateNumbers) const
    {
        checkDb();
        QString result(ENUM_TEMPLATE);

        result.replace("%class%", type->isStrong() ? "class " : "");
        result.replace("%name%",  type->name());

        QString typeName("");
        QString typeId(type->enumTypeId());
        if (typeId != STUB_ID) {
            auto t = utility::findType(m_GlobalDatabase, m_ProjectDatabase, typeId);
            if (t)  typeName.append(" : ").append(t->name());
        }
        result.replace("%type%", typeName);

        QStringList values;
        if (generateNumbers) {
            for (auto &&v : type->variables())
                values << QString("%1 = %2").arg(v.first, QString::number(v.second));
        } else {
            for (auto &&v : type->variables())
                values << v.first;
        }
        result.replace("%values%", values.isEmpty() ? "" : values.join(", "));

        return result;
    }

    QString ProjectTranslator::generateCode(const entity::SharedExtendedType &type, bool alias) const
    {
        checkDb();
        QString result(EXT_TYPE_TEMPLATE);

        result.replace("%const%", type->isConst() ? "const " : "");

        if (type->typeId() != STUB_ID) {
            auto t = utility::findType(m_GlobalDatabase, m_ProjectDatabase, type->typeId());
            result.replace("%name%", t ? this->generateCode(t) : "");
        } else {
            result.remove("%name%");
        }

        QString pl("");
        if (!type->pl().isEmpty()) {
            for (auto &&c : type->pl()) {
                pl.append(c.first);
                if (c.second) pl.append(" const ");
            }
            pl = pl.trimmed();
            pl.prepend(" ");
        }
        result.replace("%pl%", pl);

        QString params("");
        if (!type->templateParameters().isEmpty()) {
            QStringList names;
            entity::SharedType t = nullptr;
            for (auto &&id : type->templateParameters()) {
                t = utility::findType(m_GlobalDatabase, m_ProjectDatabase, id);
                if (t) names << t->name();
            }
            params = names.join(", ");
            params.append(">");
            params.prepend("<");
        }
        result.replace("%template_params%", params);

        if (alias && type->name() != DEFAULT_NAME)
            result.prepend(QString("using %1 = ").arg(type->name())).append(";");

        return result;
    }

    QString ProjectTranslator::generateCode(const entity::SharedField &type) const
    {
        checkDb();
        QString result(FIELD_TEMPLATE);

        QStringList keywords;
        if (!type->keywords().isEmpty())
            for (auto &&keyword : type->keywords())
                keywords << utility::fieldKeywordToString(keyword);
        result.replace("%keywords%", keywords.isEmpty() ? "" : keywords.join(" ").append(" "));

        auto t = utility::findType(m_GlobalDatabase, m_ProjectDatabase, type->typeId());
        entity::SharedExtendedType st = nullptr;
        if (t->type() == entity::ExtendedTypeType)
            st = std::dynamic_pointer_cast<entity::ExtendedType>(t);
        result.replace("%type%", st ? generateCode(st).append(" ") :
                                      t ? generateCode(t).append(" ") :
                                          "");
        result.replace("%name%", type->fullName());

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
