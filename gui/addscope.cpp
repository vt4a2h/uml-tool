/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 26/02/2015.
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
#include "addscope.h"
#include "ui_addscope.h"

namespace {
    const QString defaultWidgetName = gui::AddScope::tr("Create new scope");
}

namespace gui {

    /**
     * @brief AddScope::AddScope
     * @param parent
     */
    AddScope::AddScope(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::AddScope)
    {
        ui->setupUi(this);
    }

    /**
     * @brief AddScope::~AddScope
     */
    AddScope::~AddScope()
    {
        delete ui;
    }

    /**
     * @brief AddScope::setProjectName
     * @param name
     */
    void AddScope::setProjectName(const QString &name)
    {
        setWindowTitle( !name.isEmpty() ? defaultWidgetName + tr(" in project %1").arg(name) : name);
    }

} // namespace gui
