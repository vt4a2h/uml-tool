/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 13/08/2015.
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

#ifndef DB_PATH
Database path should be defined.
#endif

#include <Helpers/GeneratorID.h>

#include <Entity/EntityFactory.h>

#include <Project/Project.h>
#include <Project/ProjectTypes.hpp>

#include <DB/Database.h>
#include <DB/DBTypes.hpp>

#include <Models/ApplicationModel.h>
#include <Models/ModelsTypes.hpp>

#include <GUI/gui_types.hpp>

const QString globalDbPath = DB_PATH"/";
const QString globalDbName = "global";

class ComponentsBase
{
protected:
    ComponentsBase() { init(); }

    void init()
    {
        m_ApplicationModel = std::make_shared<Models::ApplicationModel>();
        m_GlobalDatabase = m_ApplicationModel->globalDatabase();
        m_GlobalDatabase->setPath(globalDbPath);
        m_GlobalDatabase->setName(globalDbName);

        initFactory(Entity::EntityFactory::instance());

        ErrorList errors;
        m_GlobalDatabase->load(errors);
        Q_ASSERT(errors.isEmpty());

        m_Project = std::make_shared<Projects::Project>("Foo Project", "fake path");
        const_cast<Helpers::GeneratorID&>(
            Helpers::GeneratorID::instance()).onCurrentProjectChanged(nullptr, m_Project);
        m_Project->setGlobalDatabase(m_GlobalDatabase);

        initFactory(Entity::EntityFactory::instance());

        m_ApplicationModel->addProject(m_Project);
        m_ApplicationModel->setCurrentProject(m_Project->name());
        Q_ASSERT(m_Project == m_ApplicationModel->currentProject());

        m_ProjectScope = m_Project->database()->scope(Common::ID::projectScopeID());
        Q_ASSERT(!!m_ProjectScope);

        m_Type = m_ProjectScope->addType<Entity::Class>("Foo");
    }

    void initFactory(const Common::ElementsFactory &factory)
    {
        auto &&ef = const_cast<Common::ElementsFactory &>(factory);
        ef.setGlobalDatabase(m_GlobalDatabase);
        ef.onProjectChanged(nullptr, m_Project);
    }

protected:
    Projects::SharedProject m_Project;
    DB::SharedDatabase m_GlobalDatabase;
    Models::SharedApplicationModel m_ApplicationModel;

    Entity::SharedScope m_ProjectScope;
    Entity::SharedType m_Type;
};
