/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 12/07/2015.
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

#include <entity/entity_types.hpp>

#include <models/models_types.hpp>
#include <models/componentsmodel.h>

namespace components {

    /// The optional entity
    struct OptionalEntity
    {
        QString errorMessage;
        entity::SharedBasicEntity resultEntity;
    };

    /// The ComponentsMaker class
    class ComponentsMaker
    {
        Q_DECLARE_TR_FUNCTIONS(ComponentsMaker)

    public:
        ComponentsMaker();
        ComponentsMaker(const models::SharedApplicationModel &model, const entity::SharedType &entity,
                        const entity::SharedScope &scope);

        bool signatureValid(const QString &signature, models::DisplayPart display);
        OptionalEntity makeComponent(const QString &signature, models::DisplayPart display);

        models::SharedApplicationModel model() const;
        void setModel(const models::SharedApplicationModel &model);

        entity::SharedType entity() const;
        void setEntity(const entity::SharedType &entity);

        entity::SharedScope scope() const;
        void setScope(const entity::SharedScope &scope);

    private:
        models::SharedApplicationModel m_Model;
        entity::SharedType m_Entity;
        entity::SharedScope m_Scope;

        QVector<QString> m_LastCaptured;
        QString m_LastSignature;

        OptionalEntity makeField();
    };

} // namespace components
