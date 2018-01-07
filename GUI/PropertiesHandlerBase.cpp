/*****************************************************************************
**
** Copyright (C) 2017 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 24.
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
#include "PropertiesHandlerBase.hpp"

#include <QtGui/QLayout>

#include "Section.h"

namespace GUI {

    /**
     * @brief PropertiesHandlerBase::PropertiesHandlerBase
     * @param sectionsLayout
     */
    PropertiesHandlerBase::PropertiesHandlerBase(QLayout &sectionsLayout)
        : m_MainLayout(sectionsLayout)
    {
    }

    /**
     * @brief PropertiesHandlerBase::layout
     * @return
     */
    QLayout &PropertiesHandlerBase::layout()
    {
        return m_MainLayout;
    }

    /**
     * @brief PropertiesHandlerBase::sections
     * @return
     */
    PropertiesHandlerBase::SectionsList PropertiesHandlerBase::sections() const
    {
        return m_Sections;
    }

    /**
     * @brief PropertiesHandlerBase::addSection
     * @param name
     * @param description
     * @param model
     * @return
     */
    PropertiesHandlerBase::SectionPtr PropertiesHandlerBase::addSection(
        const QString &name, const QString &description, const Models::SharedSectionalDataModel &model)
    {
        auto section = new Section(name, description, model);

        layout().addWidget(section);
        m_Sections.append(section);

        return section;
    }

    /**
     * @brief PropertiesHandlerBase::entity
     * @return
     */
    Entity::SharedType PropertiesHandlerBase::entity() const
    {
        return nullptr;
    }

    /**
     * @brief PropertiesHandlerBase::activate
     * @return
     */
    bool PropertiesHandlerBase::activate()
    {
        if (!entity() || m_Sections.isEmpty())
            return false;

        for (auto && s : sections()) {
            s->setVisible(true);
        }

        return true;
    }

    /**
     * @brief PropertiesHandlerBase::deactivate
     * @return
     */
    bool PropertiesHandlerBase::deactivate()
    {
        setEntity(nullptr);

        for (auto && s : sections()) {
            s->setVisible(false);
        }

        return true;
    }

    /**
     * @brief PropertiesHandlerBase::setEntity
     */
    void PropertiesHandlerBase::setEntity(const Entity::SharedType &/*type*/)
    {
    }

} // namespace GUI
