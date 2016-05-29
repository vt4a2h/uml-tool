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

#include <helpers/generatorid.h>

#include <project/Project.h>
#include <project/project_types.hpp>

#include <db/Database.h>
#include <db/db_types.hpp>

#include <models/applicationmodel.h>
#include <models/models_types.hpp>

#include <gui/gui_types.hpp>

const QString globalDbPath = "../../";
const QString globalDbName = "global";

class ComponentsBase
{
protected:
    ComponentsBase() { init(); }

    void init()
    {
        m_ApplicationModel = std::make_shared<models::ApplicationModel>();
        m_GlobalDatabase = m_ApplicationModel->globalDatabase();
        m_GlobalDatabase->setPath(globalDbPath);
        m_GlobalDatabase->setName(globalDbName);
        ErrorList errors;
        m_GlobalDatabase->load(errors);
        Q_ASSERT(errors.isEmpty());

        m_Project = std::make_shared<project::Project>("Foo Project", "fake path");
        const_cast<helpers::GeneratorID&>(helpers::GeneratorID::instance()).onCurrentProjectChanged(m_Project);
        m_Project->setGlobalDatabase(m_GlobalDatabase);

        m_ApplicationModel->addProject(m_Project);
        m_ApplicationModel->setCurrentProject(m_Project->name());
        Q_ASSERT(m_Project == m_ApplicationModel->currentProject());

        m_ProjectScope = m_Project->database()->scope(common::ID::projectScopeID());
        Q_ASSERT(!!m_ProjectScope);

        m_Type = m_ProjectScope->addType<entity::Class>("Foo");
    }

protected:
    project::SharedProject m_Project;
    db::SharedDatabase m_GlobalDatabase;
    models::SharedApplicationModel m_ApplicationModel;

    entity::SharedScope m_ProjectScope;
    entity::SharedType m_Type;
};
