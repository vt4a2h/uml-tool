#pragma once

#include "types.h"

namespace translator {

    class ProjectTranslator
    {
    public:
        ProjectTranslator();
        ProjectTranslator(const db::SharedDatabase &globalDb,
                          const db::SharedDatabase &projectDb);

        db::SharedDatabase globalDatabase() const;
        void setGlobalDatabase(const db::SharedDatabase &globalDatabase);

        db::SharedDatabase projectDatabase() const;
        void setProjectDatabase(const db::SharedDatabase &projectDatabase);

        QString generateCode(const entity::SharedType &type) const;
        QString generateCode(const entity::SharedEnum &type,
                             bool generateNumbers = false) const;
        QString generateCode(const entity::SharedExtendedType &type,
                             bool alias = false) const;
        QString generateCode(const entity::SharedField &type) const;

    private:
        void checkDb() const;

        db::SharedDatabase m_GlobalDatabase;
        db::SharedDatabase m_ProjectDatabase;
    };

} // namespace translator
