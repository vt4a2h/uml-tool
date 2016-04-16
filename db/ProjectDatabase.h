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
#pragma once

#include <QObject>

#include "Database.h"
#include "db_types.hpp"

#include <relationship/relationship_types.hpp>
#include <gui/graphics/graphics_types.h>

namespace db {

    /**
     * @brief The ProjectDatabase class
     */
    class ProjectDatabase : public QObject, public Database
    {
        Q_OBJECT

    public:
        ProjectDatabase(ProjectDatabase &&src);
        ProjectDatabase(const ProjectDatabase &src);
        ProjectDatabase(const QString &name = "", const QString &path = "");

        friend bool operator ==(const ProjectDatabase &lhs, const ProjectDatabase &rhs);

        ProjectDatabase &operator =(ProjectDatabase &&rhs);
        ProjectDatabase &operator =(ProjectDatabase rhs);

        relationship::SharedRelation getRelation(const common::ID &id) const;
        void addRelation(const relationship::SharedRelation &relation);
        bool containsRelation(const common::ID &id) const;
        void removeRelation(const common::ID &id);
        relationship::RelationsList relations() const;

        graphics::EntityPtr graphicsEntity(const common::ID &id) const;
        void registerGraphicsEntity(const graphics::EntityPtr &e);
        void unregisterGraphicsEntity(const graphics::EntityPtr &e);

        db::SharedDatabase globalDatabase() const;
        void setGlobalDatabase(const db::SharedDatabase &globalDatabase);

        void clear() override;

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        bool isEqual(const ProjectDatabase &rhs) const;

    signals:
        void relationAdded();
        void relationRemoved();

        void graphicsEntityRegistred(const graphics::EntityPtr &);
        void graphicsEntityUnregistred(const graphics::EntityPtr &);

    public slots:
        void onTypeUserAdded(const entity::SharedTypeUser &tu);
        // TODO: add signals to detect adding/ removing relation in order to touch project

    public: // Database overrides
        entity::SharedScope addScope(const QString &name,
                                     const common::ID &parentScopeId = common::ID::nullID()) override;
        entity::SharedScope addExistsScope(const entity::SharedScope &scope) override;

    protected:
        virtual void copyFrom(const ProjectDatabase &src);
        virtual void moveFrom(ProjectDatabase &src);

    private:
        void installTypeSearchers();

        relationship::Relations m_Relations;
        graphics::EntityHashMap m_GraphicsEntities;

        db::SharedDatabase m_GlobalDatabase;
    };

} // namespace db
