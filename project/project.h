#pragma once

#include <QString>

#include "types.h"

namespace project {

    class Project
    {
    public:
        Project();
        Project(const QString &name, const QString &path);

        QString name() const;
        void setName(const QString &name);

        QString path() const;
        void setPath(const QString &path);

        QString id() const;

        void load();
        void save();

    protected:
        QString m_Name;
        QString m_Path;
        QString m_ID;

        db::SharedProjectDatabase m_Database;
    };

} // namespace project
