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
#include "DBTypes.hpp"

#include <Relationship/relationship_types.hpp>
#include <GUI/graphics/GraphicsTypes.h>

namespace DB {

    /**
     * @brief The ProjectDatabase class
     */
    class ProjectDatabase : public Database
    {
        Q_OBJECT

    public:
        ProjectDatabase(ProjectDatabase &&src);
        ProjectDatabase(const ProjectDatabase &src);
        ProjectDatabase(const QString &name = "", const QString &path = "");

        ~ProjectDatabase() override;

        friend bool operator ==(const ProjectDatabase &lhs, const ProjectDatabase &rhs);

        ProjectDatabase &operator =(ProjectDatabase &&rhs);
        ProjectDatabase &operator =(ProjectDatabase rhs);

        Relationship::SharedRelation getRelation(const Common::ID &id) const;
        void addRelation(const Relationship::SharedRelation &relation);
        bool containsRelation(const Common::ID &id) const;
        void removeRelation(const Common::ID &id);
        Relationship::RelationsList relations() const;

        Graphics::RelationPtr graphicRelation(const Common::ID &id) const;
        void registerGraphicsRelation(const Graphics::RelationPtr &r);
        void unregisterGraphicsRelation(const Graphics::RelationPtr &r);
        Graphics::RelationVector graphicsRelations() const;

        Graphics::EntityPtr graphicsEntity(const Common::ID &id) const;
        void registerGraphicsEntity(const Graphics::EntityPtr &e);
        void unregisterGraphicsEntity(const Graphics::EntityPtr &e);
        Graphics::EntityVector graphicsEntities() const;

        Graphics::GraphicItems graphicsItems() const;

        DB::SharedDatabase globalDatabase() const;
        void setGlobalDatabase(const DB::SharedDatabase &globalDatabase);

        void clear() override;

        QJsonObject toJson() const override;
        void fromJson(const QJsonObject &src, QStringList &errorList) override;

        bool isEqual(const ProjectDatabase &rhs) const;

    signals:
        void relationAdded();
        void relationRemoved();

        // FIXME: also handle changing ID for graphic items
        void graphicsEntityRegistred(const Graphics::EntityPtr &);
        void graphicsEntityUnregistred(const Graphics::EntityPtr &);

    public slots:
        void onTypeUserAdded(const Entity::SharedTypeUser &tu);
        void onRelationIDChanged(const Common::ID &oldID, const Common::ID &newID);

    public: // Database overrides
        Entity::SharedScope addScope(const QString &name,
                                     const Common::ID &parentScopeId = Common::ID::nullID()) override;
        Entity::SharedScope addExistsScope(const Entity::SharedScope &scope) override;

        bool getClearGraphics() const;
        void setClearGraphics(bool clear);

    protected:
        virtual void copyFrom(const ProjectDatabase &src);
        virtual void moveFrom(ProjectDatabase &src);

    private:
        void installTypeSearchers();

        Relationship::Relations m_Relations;
        Graphics::EntityHashMap m_GraphicsEntities;
        Graphics::RelationHashMap m_GraphicsRelations;
        bool m_ClearGraphics = false;

        DB::SharedDatabase m_GlobalDatabase;
    };

} // namespace db

