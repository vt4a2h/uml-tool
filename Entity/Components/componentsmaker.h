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

#include <Entity/EntityTypes.hpp>
#include <Common/common_types.h>

#include <Models/ModelsTypes.hpp>
#include <Models/ComponentsModel.h>

#include <Entity/Components/components_types.h>

namespace Components {

    /// The optional entity
    struct OptionalEntity
    {
        QString errorMessage;
        Common::SharedBasicEntity resultEntity;
    };

    /// The ComponentsMaker class
    class ComponentsMaker
    {
        Q_DECLARE_TR_FUNCTIONS(ComponentsMaker)

    public:
        ComponentsMaker();
        ComponentsMaker(const Models::SharedApplicationModel &model, const Entity::SharedType &entity,
                        const Entity::SharedScope &scope);

        OptionalEntity makeComponent(const Components::Tokens &tokens, Models::DisplayPart display);

        Models::SharedApplicationModel model() const;
        void setModel(const Models::SharedApplicationModel &model);

        Entity::SharedType entity() const;
        void setEntity(const Entity::SharedType &entity);

        Entity::SharedScope scope() const;
        void setScope(const Entity::SharedScope &scope);

    private:
        using MakerFunction = std::function<OptionalEntity(const Components::Tokens &)>;
        using MakerMap = QMap<Models::DisplayPart, MakerFunction>;

        Models::SharedApplicationModel m_Model;
        Entity::SharedType m_Entity;
        Entity::SharedScope m_Scope;
        MakerMap m_ComponentMakerMap;

        bool checkCommonState() const;

        OptionalEntity makeType(const Components::Tokens &tokens);
        OptionalEntity makeField(const Components::Tokens &tokens);
        OptionalEntity makeMethod(const Components::Tokens &tokens);
        OptionalEntity makeProperty(const Components::Tokens &tokens);
    };

} // namespace components
