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

#include <QDebug>

namespace GUI {

    static const QString enumDeclarationHelp =
        QT_TRANSLATE_NOOP("EntityProperties",
                          "Enter enum declaration in the following format:<br>"
                          "enum-key<sub>opt</sub> identifier type<sub>opt</sub><br>"
                          "For example: class Foo int");
    static const QString enumEnumeratorsHelp =
        QT_TRANSLATE_NOOP("EntityProperties",
                          "Enter enumerators in the following format:<br>"
                          "enumerator constexpr<br>"
                          "enumerator constexpr<br>"
                          "..."
                          "enumerator constexpr<br><br>"
                          "For example:<br>"
                          "a 1<br>"
                          "b 2<br>");

    EntityProperties::EntityProperties(QWidget *parent)
        : QWidget(parent)
        , m_ui(new Ui::EntityProperties)
        , m_EnumDeclaration(new Section(tr("Declaration"), enumDeclarationHelp, this))
        , m_EnumElements(new Section(tr("Enumerators"), enumEnumeratorsHelp,this))
    {
        m_ui->setupUi(this);

        init();
    }

    EntityProperties::~EntityProperties()
    {
    }

    void EntityProperties::onSelectedElementsChanged(const Entity::TypesList &types)
    {
        for (auto && w : m_AllSections)
            w->setVisible(false);

        if (!types.isEmpty() && types.size() == 1)
            changeState(types[0], types[0]);
        else
            setDisabled(true);
    }

    void EntityProperties::changeState(const Entity::SharedType &type,
                                       const Entity::SharedComponents &components)
    {
        setEnabled(true);
        switch (type->kindOfType()) {
            case Entity::KindOfType::Enum:

                break;

            default:
                break;
        }
    }

    void EntityProperties::init()
    {
        // Add enum sections
        m_ui->vl->addWidget(m_EnumDeclaration.data());
        m_ui->vl->addWidget(m_EnumElements.data());
        m_EnumWidgets << m_EnumDeclaration.data() << m_EnumElements.data();

        m_AllSections << m_EnumWidgets;
    }

} // namespace GUI
