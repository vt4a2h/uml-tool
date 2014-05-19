#include "projecttranslator.h"
#include "enum.h"
#include "database.h"
#include "projectdatabase.h"
#include "templates.cpp"
#include "constants.cpp"

namespace translator {

    ProjectTranslator::ProjectTranslator(const db::SharedDatabase &globalDb, const db::SharedDatabase &projectDb)
        : m_GlobalDatabase(globalDb)
        , m_ProjectDatabase(projectDb)
    {
        checkDb();
    }

    void ProjectTranslator::checkDb()
    {
        Q_ASSERT_X(m_GlobalDatabase, "ProjectTranslator", "global example database not found");
        Q_ASSERT_X(m_ProjectDatabase, "ProjectTranslator", "project database not found");
    }

    QString ProjectTranslator::generateCode(const entity::SharedEnum &type) const
    {
        QString result(ENUM_TEMPLATE);

        result.replace("%class%", type->isStrong() ? "class" : "");
        result.replace("%name%",  type->name());

        QString typeName("");
        QString typeId(type->enumTypeId());
        if (typeId != STUB_ID) {
            auto t = m_ProjectDatabase->depthTypeSearch(typeId);
            if (!t) t = m_GlobalDatabase->depthTypeSearch(typeId);
            if (t)  typeName.append(": ").append(t->name());
        }
        result.replace("%type%", typeName);

        QStringList values;
        if (type->isOrdered() && type->variables().begin()->second == 0) {
            for (auto v : type->variables())
                values << v.first;
        } else {
            for (auto v : type->variables())
                values << QString("%1 = %2").arg(v.first, v.second);
        }
        result.replace("%values%", values.join(", "));

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


    db::SharedDatabase ProjectTranslator::globalDatabase() const
    {
        return m_GlobalDatabase;
    }

    void ProjectTranslator::setGlobalDatabase(const db::SharedDatabase &globalDatabase)
    {
        m_GlobalDatabase = globalDatabase;
    }

} // namespace translator
