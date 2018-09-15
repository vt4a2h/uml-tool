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
#pragma once

#include <QCoreApplication>

#include <Entity/Type.h>
#include <Project/ProjectTypes.hpp>

#include "IPropertiesHandler.hpp"

namespace GUI {

    class Section;

    class PropertiesHandlerBase : public QObject, public IPropertiesHandler
    {
        Q_OBJECT

    public: // Types
        using SectionPtr   = QPointer<Section>;

    public:
        PropertiesHandlerBase(QLayout &sectionsLayout, Models::SectionalTextConverter &converter);

    public: // IPropertiesHandler interface
        bool activate() override;
        bool deactivate() override;

        void setEntity(const Entity::SharedType &entity) override;

        /// Check if current changes are saved
        bool isSaved() const override;

        /// Save current changes
        void save() override;

    signals:
        void sceneUpdateRequired(const QRectF &rect);

    protected: // Metods
        QLayout &layout();

        SectionPtr currentSection();
        void setCurrentSection(SectionPtr section);

        SectionPtr addSection(const QString &name, const QString &description,
                              Entity::KindOfType type);

        Entity::SharedType entity() const;

    private: // Data
        QLayout &m_MainLayout;
        Models::SectionalTextConverter &m_Converter;

        QHash<Entity::KindOfType, SectionPtr> m_SectionsByType;

        Entity::SharedType m_Entity;

        Projects::SharedProject m_CurrentProject;

        QMetaObject::Connection m_SaveConnection;
    };

} // namespace GUI
