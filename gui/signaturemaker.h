/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 03/06/2015.
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

#include <QCoreApplication>

#include <models/models_types.hpp>
#include <entity/entity_types.hpp>
#include <project/project_types.hpp>
#include <translator/translator_types.hpp>

namespace gui {

    /// The SignatureMaker class
    class SignatureMaker
    {
        Q_DECLARE_TR_FUNCTIONS(SignatureMaker)

    public:
        SignatureMaker();
        SignatureMaker(const models::SharedApplicationModel &model, const project::SharedProject &project,
                       const entity::SharedScope &scope, const entity::SharedType &type);

        ~SignatureMaker();

        QString signature(const entity::SharedBasicEntity &component);

        entity::SharedType type() const;
        void setType(const entity::SharedType &type);

        entity::SharedScope scope() const;
        void setScope(const entity::SharedScope &scope);

        project::SharedProject project() const;
        void setProject(const project::SharedProject &project);

        models::SharedApplicationModel applicationModel() const;
        void setApplicationModel(const models::SharedApplicationModel &applicationModel);

    private:
        entity::SharedType  m_Type;
        entity::SharedScope m_Scope;
        project::SharedProject m_Project;
        models::SharedApplicationModel m_ApplicationModel;
        translator::UniqueTranslator m_Translator;
    };

} // namespace gui
