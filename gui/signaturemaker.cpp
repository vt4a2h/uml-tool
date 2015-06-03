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
#include "signaturemaker.h"

namespace gui {

    /**
     * @brief SignatureMaker::SignatureMaker
     * @param model
     * @param project
     * @param scope
     * @param type
     */
    SignatureMaker::SignatureMaker(const models::SharedApplicationModal &model, const project::SharedProject &project,
                                   const entity::SharedScope &scope, const entity::SharedType &type)
        : m_Type(type)
        , m_Scope(scope)
        , m_Project(project)
        , m_ApplicationModel(model)
    {
        Q_ASSERT(m_Type);
        Q_ASSERT(m_Scope);
        Q_ASSERT(m_Project);
        Q_ASSERT(m_ApplicationModel);
    }

    /**
     * @brief SignatureMaker::signature
     * @param entity
     * @return
     */
    QString SignatureMaker::signature(const entity::SharedBasicEntity &entity)
    {
        Q_UNUSED(entity);
        return "stub";
    }

} // namespace gui

