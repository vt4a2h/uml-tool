/*****************************************************************************
**
** Copyright (C) 2017 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 17.
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
#include "EntityProperties.h"
#include "ui_EntityProperties.h"

#include <Entity/Type.h>
#include <Entity/Enum.h>

#include "Section.h"
#include "EnumPropertiesHandler.hpp"

#include "QtHelpers.h"

#include <QDebug>

namespace GUI {

    EntityProperties::EntityProperties(QWidget *parent)
        : QWidget(parent)
        , m_ui(new Ui::EntityProperties)
    {
        m_ui->setupUi(this);

        m_PropHandlersByType[Entity::KindOfType::Enum] = std::make_shared<EnumPropHandler>(*m_ui->vl);
    }

    EntityProperties::~EntityProperties()
    {
    }

    void EntityProperties::onSelectedElementsChanged(const Entity::TypesList &types)
    {
        if (m_ActiveHandler) {
            m_ActiveHandler->deactivate();
            m_ActiveHandler.reset();
        }

        if (types.size() == 1) {
            auto type = types[0];
            auto it = m_PropHandlersByType.find(type->kindOfType());
            if (it != std::end(m_PropHandlersByType)) {
                (*it)->setEntity(type);
                G_ASSERT((*it)->activate());

                m_ActiveHandler = *it;
            }
        }
    }

} // namespace GUI
