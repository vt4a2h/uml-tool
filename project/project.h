#pragma once

#include <QString>

#include "types.h"

/**
 * @brief project
 */
namespace project {

    /**
     * @brief The Project class
     */
    class Project
    {
    public:
        Project();
        Project(const QString &name, const QString &path, const SharedErrorList &errors);
        virtual ~Project() {}

        friend bool operator ==(const Project &lhs, const Project &rhs);

        QString name() const;
        void setName(const QString &name);

        QString path() const;
        void setPath(const QString &path);

        QString id() const;

        void load(const QString &path); // don't forget install global database after load
        void save();

        db::SharedProjectDatabase database() const;
        void setDatabase(const db::SharedProjectDatabase &database);

        db::SharedDatabase globalDatabase() const;
        bool setGloablDatabase(const db::SharedDatabase &database);

        bool anyErrors() const;
        SharedErrorList errors() const;
        void setErrorsList(const SharedErrorList &errors);

        virtual QJsonObject toJson() const;
        virtual void fromJson(const QJsonObject &src, QStringList &errorList);

    protected:
        virtual QString projectFileName() const;
        virtual QString databaseFileName() const;
        virtual QString projectPath(const QString &basePath) const;
        virtual QString databasePath(const QString &basePath) const;

        QString m_Name;
        QString m_Path;
        QString m_ID;

        db::SharedProjectDatabase m_Database;
        SharedErrorList m_Errors;
    };

} // namespace project
